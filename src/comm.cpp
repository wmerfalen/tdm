/* ************************************************************************
 *   File: comm.c                                        Part of CircleMUD *
 *  Usage: Communication, socket handling, main(), central game loop       *
 *                                                                         *
 *  All rights reserved.  See license.doc for complete information.        *
 *                                                                         *
 *  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
 *  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
 ************************************************************************ */

#define __COMM_C__

#include "globals.hpp"
#include <array>
#include "conf.h"
#include "sysdep.h"
#include "mods/extern.hpp"
#include "mods/date-time.hpp"
#include <sys/epoll.h>
#include "mods/debug.hpp"
#include "mods/loops.hpp"
#include "mods/affects.hpp"
#include "mods/rooms.hpp"
#include <algorithm>
#include "mods/world-configuration.hpp"
#include "mods/auto-login.hpp"
#include "mods/zone.hpp"
#include "mods/replenish.hpp"
#include "mods/memory.hpp"
#include "mods/orm/inventory.hpp"
#include "mods/players/db-load.hpp"
#include "mods/players/friendly-reminders.hpp"
#include "mods/players/messages.hpp"
#include "mods/scripted-sequence-runner.hpp"
#include "mods/js.hpp"
#include "mods/corrosive.hpp"
#include "mods/bleed.hpp"
#include "mods/blind.hpp"
#include "mods/suppress.hpp"
#include "mods/terrify.hpp"
#include "mods/melt.hpp"
#include "mods/ensnare.hpp"
#include "mods/resting.hpp"
#include "mods/mobs/behaviour-tree-list.hpp"
#include "mods/mobs/defiler-behaviour-tree.hpp"
#include "mods/mobs/orthos-spawn-sentinel-btree.hpp"
#include "mods/players/event-messages.hpp"
#include "mods/message-server.hpp"
#include "mods/ban-system.hpp"
#include "mods/message-server.hpp"
#include "mods/mobs/orthos-spawn-sentinel-btree.hpp"
#include "mods/mobs/goat.hpp"
#include "mods/loot.hpp"
namespace mods::mobs::room_watching {
	extern void destroy_player(uuid_t);
	extern void heartbeat();
};

#if CIRCLE_GNU_LIBC_MEMORY_TRACK
	#include <mcheck.h>
#endif

#ifdef CIRCLE_MACINTOSH		/* Includes for the Macintosh */
	#define SIGPIPE 13
	#define SIGALRM 14
	/* GUSI headers */
	#include <sys/ioctl.h>
	/* Codewarrior dependant */
	#include <SIOUX.h>
	#include <console.h>
#endif

#ifdef CIRCLE_WINDOWS		/* Includes for Win32 */
	#ifdef __BORLANDC__
		#include <dir.h>
	#else /* MSVC */
		#include <direct.h>
	#endif
	#include <mmsystem.h>
#endif /* CIRCLE_WINDOWS */

#ifdef CIRCLE_AMIGA		/* Includes for the Amiga */
	#include <sys/ioctl.h>
	#include <clib/socket_protos.h>
#endif /* CIRCLE_AMIGA */

#ifdef CIRCLE_ACORN		/* Includes for the Acorn (RiscOS) */
	#include <socklib.h>
	#include <inetlib.h>
	#include <sys/ioctl.h>
#endif
/*
 * Note, most includes for all platforms are in sysdep.h.  The list of
 * files that is included is controlled by conf.h for that platform.
 */
#define __MENTOC_USE_LEGACY_SLEEP_CODE__

#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "house.h"

#ifdef HAVE_ARPA_TELNET_H
	#include <arpa/telnet.h>
#else
	#include "telnet.h"
#endif

#ifndef INVALID_SOCKET
	#define INVALID_SOCKET (-1)
#endif

#include "mods/acl/config-parser.hpp" 		/** Access Control List */
#include "mods/behaviour_tree_impl.hpp"
#include "globals.hpp"

/* externs */
extern std::string GREETINGS;
extern const char *circlemud_version;
extern int circle_restrict;
extern int mini_mud;
extern int no_rent_check;
extern void do_start(char_data *ch);

extern ush_int DFLT_PORT;
extern const char *DFLT_DIR;
extern const char *DFLT_IP;
extern const char *LOGNAME;
extern int max_playing;
extern int nameserver_is_slow;	/* see config.c */
extern int auto_save;		/* see config.c */
extern int autosave_time;	/* see config.c */
extern int *cmd_sort_info;
namespace mods::globals {
	extern std::vector<std::vector<player_ptr_t>> room_list;
};

extern struct time_info_data time_info;		/* In db.c */
extern char *help;
player_ptr_t new_player() {
	return mods::globals::player_list.emplace_back(std::make_shared<mods::player>(mods::player::player_type_enum_t::PLAYER));
}

using descriptor_list_t = std::deque<mods::descriptor_data>;
descriptor_list_t descriptor_list;		/* master desc list */
/* local globals */
struct txt_block *bufpool = 0;	/* pool of large output buffers */
int buf_largecount = 0;		/* # of large buffers which exist */
int buf_overflows = 0;		/* # of overflows of output */
int buf_switches = 0;		/* # of switches from small to large buf */
int circle_shutdown = 0;	/* clean shutdown */
int circle_reboot = 0;		/* reboot the game after a shutdown */
int no_specials = 0;		/* Suppress special routines */
int max_players = 0;		/* max descriptors available */
uint64_t tics = 0;			/* for extern checkpointing */
int scheck = 0;			/* for syntax checking mode */
struct timeval null_time;	/* zero-valued time structure */
byte reread_wizlist;		/* signal: SIGUSR1 */
byte emergency_unban;		/* signal: SIGUSR2 */
FILE *logfile = NULL;		/* Where to send the log messages. */
const char *text_overflow = "**OVERFLOW**\r\n";
int epoll_fd = -1;
epoll_event epoll_ev;

void on_shutdown() {
	if(mods::globals::db) {
		mods::globals::db->abort_txn();
		mods::globals::db->close();
		mods::globals::db.release();
	}
}
void logstrerror(std::string_view prefix,int _errno) {
	log(mods::string(prefix.data()) + strerror(_errno));
}

int destroy_player(player_ptr_t&& player);
std::size_t handle_disconnects() {
	/* Kick out folks in the CON_CLOSE or CON_DISCONNECT state */
	std::vector<typename mods::globals::player_list_t::iterator> players_to_destroy;
	for(auto it = mods::globals::player_list.begin();
	    it != mods::globals::player_list.end(); ++it) {
		auto desc = (*it)->desc();
		if(STATE(desc) == CON_CLOSE || STATE(desc) == CON_DISCONNECT) {
			players_to_destroy.push_back(it);
		}
	}

	std::set<std::string> saved_players;
	for(auto player_iterator : players_to_destroy) {
		destroy_player(std::move(*player_iterator));
	}
	auto ret = players_to_destroy.size();
	players_to_destroy.clear();
#ifdef __MENTOC_SHOW_REMAINING_PLAYER_LIST_DEBUG_OUTPUT__
	for(auto& player : mods::globals::player_list) {
		std::cerr << "[handle_disconnects][player listing][name]:'" << player->name().c_str() << "', db_id:'" << player->db_id() << "'\n";
#
	}
#endif
	return ret;
}


/* functions in this file */
RETSIGTYPE reread_wizlists(int sig);
RETSIGTYPE unrestrict_game(int sig);
RETSIGTYPE reap(int sig);
RETSIGTYPE checkpointing(int sig);
RETSIGTYPE hupsig(int sig);
ssize_t perform_socket_read(socket_t desc, char *read_point,size_t space_left);
ssize_t perform_socket_write(socket_t desc, const char *txt,size_t length);
void echo_off(mods::descriptor_data& d);
void echo_on(mods::descriptor_data& d);
void circle_sleep(struct timeval *timeout);
int get_from_q(mods::descriptor_data& d, char *dest, int *aliased);
void init_game(ush_int port);
void signal_setup(void);
void game_loop(socket_t mother_desc);
socket_t init_socket(ush_int port);
int new_descriptor(socket_t s);

static int constexpr MENTOC_PLAYER_IS_BANNED = -1;
static int constexpr MENTOC_FULL_SERVER = -2;
static int constexpr MENTOC_SEND_BUF_FAILED = -3;
static int constexpr MENTOC_INVALID_SOCKET = -4;


int get_max_players(void);
int process_output(mods::descriptor_data& in_t);
int process_input(mods::descriptor_data& t);
void timediff(struct timeval *diff, struct timeval *a, struct timeval *b);
void timeadd(struct timeval *sum, struct timeval *a, struct timeval *b);
void flush_queues(mods::descriptor_data& d);
void nonblock(socket_t s);
int perform_subst(mods::descriptor_data& t, char *orig, char *subst);
void record_usage(void);
void send_prompt(player_ptr_t& player);
void check_idle_passwords(void);
void heartbeat(int pulse);
struct in_addr *get_bind_addr(void);
int parse_ip(const char *addr, struct in_addr *inaddr);
int set_sendbuf(socket_t s);
void setup_log(const char *filename, int fd);
int open_logfile(const char *filename, FILE *stderr_fp);
#if defined(POSIX)
	sigfunc *my_signal(int signo, sigfunc *func);
#endif

/* extern fcnts */
void reboot_wizlists(void);
void boot_world(void);
void affect_update(void);	/* In magic.c */
void mobile_activity(void);
void perform_violence(void);
void show_string(mods::descriptor_data d, char *input);
int isbanned(const char *hostname);
void weather_and_time(int mode);
int perform_alias(mods::descriptor_data& d, char *orig, size_t maxlen);
void clear_free_list(void);
void free_messages(void);
void Board_clear_all(void);
void free_social_messages(void);

void deregister_player(player_ptr_t player_obj) {
	std::set<mods::globals::player_list_t::iterator> players_to_destroy;
	for(auto it = mods::globals::player_list.begin(); it != mods::globals::player_list.end(); ++it) {
		if((*it)->desc().descriptor == player_obj->desc().descriptor) {
			players_to_destroy.insert(it);
		}
	}
	for(auto& it : players_to_destroy) {
		mods::globals::player_list.erase(it);
	}
	return;
}
namespace mods::bugs {
	extern void close();
};

void atexit_handler() {
	mods::bugs::close();
	mods::db_report::close();
}

void exit_with(int code) {
	mods::bugs::close();
	mods::db_report::close();
	circle_shutdown = 1;
}

void run_game() {
	ush_int port;
	const char *dir;

#if CIRCLE_GNU_LIBC_MEMORY_TRACK
	mtrace();	/* This must come before any use of malloc(). */
#endif

	port = DFLT_PORT;
	dir = DFLT_DIR;

	/* All arguments have been parsed, try to open log file. */
	setup_log(LOGNAME, STDERR_FILENO);

	/*
	 * Moved here to distinguish command line options and to show up
	 * in the log if stderr is redirected to a file.
	 */
	log("%s", circlemud_version);

	if(chdir(dir) < 0) {
		perror("SYSERR: Fatal error changing to data directory");
		exit_with(1);
	}

	log("Using %s as data directory.", dir);

	if(scheck) {
		boot_world();
	} else {
		log("Running game on port %d.", port);
		init_game(port);
	}

	if(!scheck) {
		log("Clearing other memory.");
		free_player_index();	/* db.c */
		free_messages();		/* fight.c */
		clear_free_list();		/* mail.c */
		free_text_files();		/* db.c */
		Board_clear_all();		/* boards.c */
		free(cmd_sort_info);	/* act.informative.c */
		free_social_messages();	/* act.social.c */
		free_help();		/* db.c */
	}

	log("Done.");
}
/***********************************************************************
 *  main game loop and related stuff                                    *
 ***********************************************************************/

int main(int argc, char **argv) {
	atexit(atexit_handler);
	if(mods::globals::init(argc,argv) == 0) {
		return 0;
	}
	mods::message_server::spawn();
	std::thread(run_game).join();

	return (0);
}



/* Init sockets, run game, and cleanup sockets */
void init_game(ush_int port) {
	db_renew_txn();
	socket_t mother_desc;

	/* We don't want to restart if we crash before we get up. */
	touch(KILLSCRIPT_FILE);

	circle_srandom(time(0));

	log("Finding player limit.");
	max_players = get_max_players();

	log("Opening mother connection.");
	mother_desc = init_socket(port);

	if(boot_type_hell()) {
		boot_hell();
	} else {
		boot_db();
	}

	log("Signal trapping.");
	signal_setup();

	/* If we made it this far, we will be able to restart without problem. */
	remove(KILLSCRIPT_FILE);

	log("Entering game loop.");

	game_loop(mother_desc);

	Crash_save_all();

	log("Closing all sockets.");

	for(auto& desc : descriptor_list) {
		close_socket(desc);
	}

	close(mother_desc);


	log("Saving current MUD time.");
	save_mud_time(&time_info);

	if(circle_reboot) {
		log("Rebooting.");
		exit_with(52);			/* what's so great about HHGTTG, anyhow? */
	}

	log("Normal termination of game.");
}



/*
 * init_socket sets up the mother descriptor - creates the socket, sets
 * its options up, binds it, and listens.
 */
socket_t init_socket(ush_int port) {
	socket_t s;
	struct sockaddr_in sa;
	int opt;

	/*
	 * Should the first argument to socket() be AF_INET or PF_INET?  I don't
	 * know, take your pick.  PF_INET seems to be more widely adopted, and
	 * Comer (_Internetworking with TCP/IP_) even makes a point to say that
	 * people erroneously use AF_INET with socket() when they should be using
	 * PF_INET.  However, the man pages of some systems indicate that AF_INET
	 * is correct; some such as ConvexOS even say that you can use either one.
	 * All implementations I've seen define AF_INET and PF_INET to be the same
	 * number anyway, so the point is (hopefully) moot.
	 */

	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("SYSERR: Error creating socket");
		exit_with(1);
	}

#if defined(SO_REUSEADDR)
	opt = 1;

	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)) < 0) {
		perror("SYSERR: setsockopt REUSEADDR");
		exit_with(1);
	}

#endif

	set_sendbuf(s);

	/*
	 * The GUSI sockets library is derived from BSD, so it defines
	 * SO_LINGER, even though setsockopt() is unimplimented.
	 *	(from Dean Takemori <dean@UHHEPH.PHYS.HAWAII.EDU>)
	 */
#if defined(SO_LINGER)
	{
		struct linger ld;

		ld.l_onoff = 0;
		ld.l_linger = 0;

		if(setsockopt(s, SOL_SOCKET, SO_LINGER, (char *) &ld, sizeof(ld)) < 0) {
			perror("SYSERR: setsockopt SO_LINGER");    /* Not fatal I suppose. */
		}
	}
#endif

	/* Clear the structure */
	memset((char *)&sa, 0, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr = *(get_bind_addr());

	if(bind(s, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		perror("SYSERR: bind");
		CLOSE_SOCKET(s);
		exit_with(1);
	}

	nonblock(s);
	listen(s, 64);
	return (s);
}


int get_max_players(void) {
#ifndef CIRCLE_UNIX
	return (max_playing);
#else

	int max_descs = 0;
	const char *method;

	/*
	 * First, we'll try using getrlimit/setrlimit.  This will probably work
	 * on most systems.  HAS_RLIMIT is defined in sysdep.h.
	 */
#ifdef HAS_RLIMIT
	{
		struct rlimit limit;

		/* find the limit of file descs */
		method = "rlimit";

		if(getrlimit(RLIMIT_NOFILE, &limit) < 0) {
			perror("SYSERR: calling getrlimit");
			exit_with(1);
		}

		/* set the current to the maximum */
		limit.rlim_cur = limit.rlim_max;

		if(setrlimit(RLIMIT_NOFILE, &limit) < 0) {
			perror("SYSERR: calling setrlimit");
			exit_with(1);
		}

#ifdef RLIM_INFINITY

		if(limit.rlim_max == RLIM_INFINITY) {
			max_descs = max_playing + NUM_RESERVED_DESCS;
		} else {
			max_descs = MIN(max_playing + NUM_RESERVED_DESCS, limit.rlim_max);
		}

#else
		max_descs = MIN(max_playing + NUM_RESERVED_DESCS, limit.rlim_max);
#endif
	}

#elif defined (OPEN_MAX) || defined(FOPEN_MAX)
#if !defined(OPEN_MAX)
#define OPEN_MAX FOPEN_MAX
#endif
	method = "OPEN_MAX";
	max_descs = OPEN_MAX;		/* Uh oh.. rlimit didn't work, but we have
													 * OPEN_MAX */
#elif defined (_SC_OPEN_MAX)
	/*
	 * Okay, you don't have getrlimit() and you don't have OPEN_MAX.  Time to
	 * try the POSIX sysconf() function.  (See Stevens' _Advanced Programming
	 * in the UNIX Environment_).
	 */
	method = "POSIX sysconf";
	errno = 0;

	if((max_descs = sysconf(_SC_OPEN_MAX)) < 0) {
		if(errno == 0) {
			max_descs = max_playing + NUM_RESERVED_DESCS;
		} else {
			perror("SYSERR: Error calling sysconf");
			exit_with(1);
		}
	}

#else
	/* if everything has failed, we'll just take a guess */
	method = "random guess";
	max_descs = max_playing + NUM_RESERVED_DESCS;
#endif

	/* now calculate max _players_ based on max descs */
	max_descs = MIN(max_playing, max_descs - NUM_RESERVED_DESCS);

	if(max_descs <= 0) {
		log("SYSERR: Non-positive max player limit!  (Set at %d using %s).",
		    max_descs, method);
		exit_with(1);
	}

	log("   Setting player limit to %d using %s.", max_descs, method);
	return (max_descs);
#endif /* CIRCLE_UNIX */
}


void perform_auto_login(player_ptr_t& player) {
	player->set_name(mods::auto_login::get_user());
	player->set_db_id(0);
	auto pw = mods::auto_login::get_password();
	if(login(mods::auto_login::get_user(),pw) == false) {
		log("SYSERR: user/password combination for auto_login failed");
		exit_with(1);
	} else {
		parse_sql_player(player);
	}
	player->set_authenticated(true);
	{
		int start_room = 0;
		if(!boot_type_hell()) {
			start_room = mods::world_conf::real_mortal_start();
		}
		if(world.size() == 0) {
			exit_with(0);
		}
		player->set_room(start_room);
		char_to_room(player->cd(),start_room);
	}
	mods::globals::register_authenticated_player(player);
	act("$n has entered the game.", TRUE, player->cd(), 0, 0, TO_ROOM);
	player->set_state(CON_PLAYING);
	player->position() = POS_STANDING;

	if(player->level() == 0) {
		do_start(player->cd());
	}

	look_at_room(player->cd(), 0);

	player->start_histfile();
	player->desc().has_prompt = 1;
#ifndef __MENTOC_DONT_RUN_PROFILE_SCRIPTS__
	mods::js::run_profile_scripts(player->name());
#endif
	mods::players::db_load::game_entry(player);
}
#define PROCESS_INPUT_CONTROL_C -10

/*
 * game_loop contains the main loop which drives the entire MUD.  It
 * cycles once every 0.10 seconds and is responsible for accepting new
 * new connections, polling existing connections for input, dequeueing
 * output and sending it out to players, and calling "heartbeat" functions
 * such as mobile_activity().
 */
void game_loop(socket_t mother_desc) {
	mods::globals::current_tick = 0;
	char comm[MAX_INPUT_LENGTH];
#ifdef __MENTOC_USE_LEGACY_SLEEP_CODE__
	struct timeval last_time, opt_time, process_time, temp_time;
	struct timeval before_sleep, now, timeout;
	int missed_pulses;
#endif
	int pulse = 0, aliased = 0;

	signal(SIGPIPE,SIG_IGN);
#ifdef __MENTOC_USE_LEGACY_SLEEP_CODE__
	/* initialize various time values */
	null_time.tv_sec = 0;
	null_time.tv_usec = 0;
	opt_time.tv_usec = OPT_USEC;
	opt_time.tv_sec = 0;
#endif
	mods::globals::pre_game_loop();

	/* The Main Loop.  The Big Cheese.  The Top Dog.  The Head Honcho.  The.. */
	pulse = 0;
	aliased = 0;
	const int size = 10; // hint
	epoll_fd = epoll_create(size);
	if(epoll_fd == -1) {
		logstrerror("SYSERR: [epoll] epoll_create failed: ",errno);
		return;
	}
	// add fd to reactor
	epoll_ev.events = EPOLLIN; // new connection is a read event
	epoll_ev.data.fd = mother_desc; // user data
	int epoll_ctl_r = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, mother_desc, &epoll_ev);
	if(epoll_ctl_r == -1) {
		logstrerror("SYSERR: [epoll] epoll_ctl failed: ", errno);
		/** !fixme: do proper shutdown here */
		close(epoll_fd);
		close(mother_desc);
		return;
	}
#ifdef __MENTOC_USE_LEGACY_SLEEP_CODE__
	gettimeofday(&last_time, (struct timezone *) 0);
#endif
	while(!circle_shutdown) {
		mods::globals::defer_queue->iteration();

		epoll_event events[size];
		constexpr int epoll_timeout = 5;	/* miliseconds */

		int epoll_wait_status = epoll_wait(epoll_fd, events, size, epoll_timeout);
		if(epoll_wait_status == -1) {
			logstrerror("SYSERR: game_loop::epoll_wait[-1]->", errno);
			continue;
		}

		int i = 0;
		int new_desc = 0;
#ifdef __MENTOC_USE_LEGACY_SLEEP_CODE__
		gettimeofday(&last_time, (struct timezone *) 0);
#endif

		while(i < epoll_wait_status) {
#ifdef __MENTOC_SHOW_EPOLL_WAIT_STATUS_MESSAGE__
			std::cerr << "[while epoll wait status]\n";
#endif
			new_desc = 0;
			auto operating_socket = events[i].data.fd;
			if(events[i].data.fd == mother_desc) {
#ifdef __MENTOC_SHOW_FD_EQUALS_MOTHER_MESSAGE__
				std::cerr << "[fd == mother desc]\n";
#endif
				new_desc = new_descriptor(mother_desc);
				if(new_desc <= 0) {
					switch(new_desc) {
						case MENTOC_PLAYER_IS_BANNED:
							log("new_descriptor: player is banned");
							break;
						case MENTOC_FULL_SERVER:
							log("new_descriptor: mud is full");
							break;
						case MENTOC_SEND_BUF_FAILED:
							log("new_descriptor: send buf failed");
							break;
						case MENTOC_INVALID_SOCKET:
							log("new_descriptor: invalid socket");
							break;
						default:
							log("new_descriptor: unknown error returned: %d",new_desc);
							break;
					}
					break;
				}
				operating_socket = new_desc;
				epoll_ev.events = EPOLLIN; // new connection is a read event
				epoll_ev.data.fd = new_desc; // user data
				int epoll_ctl_add_new = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_desc, &epoll_ev);
				if(epoll_ctl_add_new == -1) {
					/** !fixme: de-reg and de-alloc player obj here -- remove from socket_map*/
					logstrerror("SYSERR:[epoll] epoll_ctl failed: ", errno);
					/** !fixme: do proper shutdown here */
					close(epoll_fd);
					close(mother_desc);
					return;
				}
			}
			auto it = mods::globals::socket_map.find(operating_socket);
			if(it == mods::globals::socket_map.end()) {
				log("WARNING: socket_map index not found for socket %d",operating_socket);
				if(-1 == epoll_ctl(epoll_fd, EPOLL_CTL_DEL, operating_socket, &epoll_ev)) {
					log("SYSERR:[epoll] epoll_ctl EPOLL_CTL_DEL removal of socket failed");
				}
				++i;
				continue;
			}

			auto player = it->second;
			mods::globals::current_player = player;

			if(mods::auto_login::auto_login_enabled() && player->state() != CON_PLAYING) {
				perform_auto_login(player);
				break;
			}
			auto input_status = process_input(player->desc());
#ifdef __MENTOC_SHOW_INPUT_STATUS_MESSAGE__
			std::cerr << green_str("input_status:") << input_status << "\n";
#endif

			if(input_status < 0) {
				switch(input_status) {
					case -2:
						log("WARNING! player has zero as socket descriptor! Destroying player [%s]",player->name().c_str());
						break;
					case -1:
						log("process_input failed for player %s",player->name().c_str());
						break;
					case -3:
						log("process_input failed for player %s. Must disconnect immediately.",player->name().c_str());
						break;
					default:
						log("process_input unknown error return: %d player [%s]",input_status,player->name().c_str());
						break;
				}
				mods::globals::current_player = nullptr;
				player->set_socket(operating_socket);
				/** TODO: destroy_player needs to properly remove from all the player refs in mods::globals */
				mods::mobs::room_watching::destroy_player(player->uuid());
				destroy_player(std::move(player));
				++i;
				continue;
			}
			aliased = 0;
			if(input_status != PROCESS_INPUT_CONTROL_C && !get_from_q(player->desc(), comm, &aliased)) {
#ifdef __MENTOC_SHOW_GET_FROM_Q_FALSEY__
				std::cerr << red_str("get_from_q returns falsey\n");
#endif
				++i;
				continue;
			}
			// Reset the idle timer & pull char back from void if necessary
			player->cd()->char_specials.timer = 0;
			/*
			 * TODO: this is breaking mortal start room and recall
			 if(player->state() == CON_PLAYING && GET_WAS_IN(player->cd()) != NOWHERE) {
			 if(player->room() != NOWHERE) {
			 char_from_room(player);
			 }
			 char_to_room(player, GET_WAS_IN(player));
			 GET_WAS_IN(player) = NOWHERE;
			 act("$n has returned.", TRUE, player->cd(), 0, 0, TO_ROOM);
			 }
			 */
			GET_WAIT_STATE(player->cd()) = 1;
			player->desc().has_prompt = false;
			if(player->state() != CON_PLAYING) { // In menus, etc.
				nanny(player, comm);
			} else {			// else: we're playing normally.
				if(aliased) {	// To prevent recursive aliases.
					player->desc().has_prompt = TRUE;    // To get newline before next cmd output.
				} else if(perform_alias(player->desc(), comm, sizeof(comm))) { // Run it through aliasing system
					get_from_q(player->desc(), comm, &aliased);
				}
				command_interpreter(player, comm); // Send it to interpreter
			}
			++i;
		}//end while(i < r)

#ifdef __MENTOC_USE_LEGACY_SLEEP_CODE__
		/*
		 * At this point, we have completed all input, output and heartbeat
		 * activity from the previous iteration, so we have to put ourselves
		 * to sleep until the next 0.1 second tick.  The first step is to
		 * calculate how long we took processing the previous iteration.
		 */

		gettimeofday(&before_sleep, (struct timezone *) 0); /* current time */
		timediff(&process_time, &before_sleep, &last_time);
		/*
		 * If we were asleep for more than one pass, count missed pulses and sleep
		 * until we're resynchronized with the next upcoming pulse.
		 */
		if(process_time.tv_sec == 0 && process_time.tv_usec < OPT_USEC) {
			missed_pulses = 0;
		} else {
			missed_pulses = process_time.tv_sec * PASSES_PER_SEC;
			missed_pulses += process_time.tv_usec / OPT_USEC;
			process_time.tv_sec = 0;
			process_time.tv_usec = process_time.tv_usec % OPT_USEC;
		}

		/* Calculate the time we should wake up */
		timediff(&temp_time, &opt_time, &process_time);
		timeadd(&last_time, &before_sleep, &temp_time);

		/* Now keep sleeping until that time has come */
		gettimeofday(&now, (struct timezone *) 0);
		timediff(&timeout, &last_time, &now);
		/* Go to sleep */
		do {
			circle_sleep(&timeout);
			gettimeofday(&now, (struct timezone *) 0);
			timediff(&timeout, &last_time, &now);
		} while(timeout.tv_usec || timeout.tv_sec);

#endif

		/** !todo: refactor this to not loop through all descriptors but to instead queue up data to be output and output them accordingly */
		for(auto& p : mods::globals::player_list) {
			if(p->desc().has_output) {
				p->desc().flush_output();
				p->desc().has_output = false;
				continue;
			}
		}

		/* Print prompts for other descriptors who had no other output */
		for(auto& p : mods::globals::player_list) {
			if(!p->desc().has_output && !p->desc().has_prompt && p->state() == CON_PLAYING) {
				send_prompt(p);
				p->desc().has_prompt = true;
				continue;
			}
		}
		//mods::players::messages::consume_all();

		handle_disconnects();
#ifdef __MENTOC_USE_LEGACY_SLEEP_CODE__
		/*
		 * Now, we execute as many pulses as necessary--just one if we haven't
		 * missed any pulses, or make up for lost time if we missed a few
		 * pulses by sleeping for too long.
		 */
		missed_pulses++;

		if(missed_pulses <= 0) {
			log("SYSERR: **BAD** MISSED_PULSES NONPOSITIVE (%d), TIME GOING BACKWARDS!!", missed_pulses);
			missed_pulses = 1;
		}

		/* If we missed more than 30 seconds worth of pulses, just do 30 secs */
		if(missed_pulses > 30 RL_SEC) {
			log("SYSERR: Missed %d seconds worth of pulses.", missed_pulses / PASSES_PER_SEC);
			missed_pulses = 30 RL_SEC;
		}

		/* Now execute the heartbeat functions */
		while(missed_pulses--) {
			heartbeat(++pulse);
		}
#else
		heartbeat(++pulse);
#endif

		/* Check for any signals we may have received. */
		if(reread_wizlist) {
			reread_wizlist = FALSE;
			mudlog(CMP, LVL_IMMORT, TRUE, "Signal received - rereading wizlists.");
			reboot_wizlists();
		}

		if(emergency_unban) {
			emergency_unban = FALSE;
			mudlog(BRF, LVL_IMMORT, TRUE, "Received SIGUSR2 - completely unrestricting game (emergent)");
			circle_restrict = 0;
		}
		if(tics + 1 == std::numeric_limits<decltype(tics)>::max()) {
			tics = 1;
		}

#ifdef __MENTOC_ALWAYS_SHOW_TICKS__
		std::cerr << "tic|";
#endif
		if(mods::debug::debug_state->show_tics()) {
			std::cerr << "tic|";
		}
		++tics;
	}
	mods::js::on_shutdown();
	on_shutdown();
}
#ifdef __MENTOC_MUTE_BEHAVIOUR_TREE_OUTPUT__
	#define rb_bht_debug(a) /**/
#else
	#define rb_bht_debug(a){ std::cerr << "[run_behaviour_trees][behaviour_trees]" << __FILE__ << "|" << __LINE__ << "->" << a << "\n"; }
#endif
void run_behaviour_trees() {
	rb_bht_debug("run_behaviour_trees [ENTRY]");
	mods::mobs::behaviour_tree_list::run();
	mods::mobs::defiler_behaviour_tree::run_trees();
	for(const auto& npc_uuid : mods::behaviour_tree_impl::mob_list()) {
		auto npc = npc_by_uuid(npc_uuid);
		if(!npc) {
			continue;
		}
		switch(npc->position()) {
			default:
				break;
			case POS_DEAD:
			case POS_MORTALLYW:
			case POS_INCAP:
			case POS_STUNNED:
				mods::behaviour_tree_impl::unregister_by_uuid(npc_uuid);
				continue;
				break;
		}
		if(npc->mob_specials().behaviour_tree) {
			auto dispatch_result = mods::behaviour_tree_impl::dispatch_ptr(*npc);
			rb_bht_debug("dispatch_result: '" << std::to_string(dispatch_result) << "'");
			switch(dispatch_result) {
				case mods::behaviour_tree_impl::dispatch_status_t::RETURN_IMMEDIATELY:
					rb_bht_debug("dispatch result: Return immediately");
					break;
				//continue;
				case mods::behaviour_tree_impl::dispatch_status_t::RETURN_FALSE_IMMEDIATELY:
					rb_bht_debug("dispatch result: Return FALSE immediately");
					break;
				//continue;
				case mods::behaviour_tree_impl::dispatch_status_t::AS_YOU_WERE:
					rb_bht_debug("dispatch result: As you were...");
					break;
				case mods::behaviour_tree_impl::dispatch_status_t::MOB_DOESNT_EXIST:
					rb_bht_debug("dispatch result: MOB doesn't exist according to behaviour tree...");
					break;
				default:
					rb_bht_debug("dispatch result: defaulted...");
					break;
			}
		}
	}
	rb_bht_debug("run_behaviour_trees [RETURN]");
}

void heartbeat(int pulse) {
	mods::date_time::heartbeat();
	static int mins_since_crashsave = 0;
	static uint16_t blind_ticks = 0;
	static uint16_t terrify_ticks = 0;
	mods::globals::current_tick++;
	if(!(pulse % mods::zone::refresh_tick_resolution())) {
		if(mods::zone::should_refresh()) {
			mods::zone::refresh_mobs_and_zones();
			mods::zone::done_refreshing();
			log("refresh_mobs_and_zones completed");
		}
	}

	if(!(pulse % 160)) {
		if(mods::loot::should_run_replenish_static_loot()) {
			log("replenish_static_loot");
			mods::loot::replenish_static_loot();
		}
	}

	if(!(pulse % mods::deferred::TICK_RESOLUTION)) {
		/** Process affect dissolver ticks */
		mods::affects::process();
		mods::rooms::affects::process();
		mods::resting::process_players_resting();
		++blind_ticks;
		++terrify_ticks;
		if(!(blind_ticks % mods::blind::tick_resolution())) {
			mods::blind::process_players();
			blind_ticks = 0;
		}
		if(!(terrify_ticks % mods::terrify::tick_resolution())) {
			mods::terrify::process_players();
			terrify_ticks = 0;
		}
	}
	if(!(pulse % mods::suppress::tick_resolution())) {
		mods::suppress::process_players();
	}

	if(!(pulse % mods::corrosive::tick_resolution())) {
		mods::corrosive::process_corrosion();
	}

	if(!(pulse % mods::bleed::tick_resolution())) {
		mods::bleed::process_bleed();
	}
	if(!(pulse % mods::ensnare::tick_resolution())) {
		mods::ensnare::process_ensnare();
	}
	if(!(pulse % mods::melt::tick_resolution())) {
		mods::melt::process_melt();
	}
	if(!(pulse % FIRE_DAMAGE_TICK_RESOLUTION())) {
		mods::rooms::process_fire_damage();
	}

	if(!(pulse % PULSE_ZONE)) {
#ifdef __MENTOC_SHOW_ZONE_PULSE_CALL_DEBUG_OUTPUT__
		std::cerr << green_str("pulse % PULSE_ZONE. calling mods::zone::zone_update()") << "\n";
#endif
		mods::zone::zone_update();
	}
	if(!(pulse % PULSE_REPLENISH_CHARACTERS)) {
		mods::replenish::run();
	}

	if(!(pulse % PULSE_IDLEPWD)) {	/* 15 seconds */
		check_idle_passwords();
	}

	constexpr static uint32_t PULSE_BEHAVIOUR_TREES = 30;
	if(!(pulse % PULSE_BEHAVIOUR_TREES)) {
		run_behaviour_trees();
	}

	if(!(pulse % mods::mobs::goat_pulse())) {
		mods::mobs::run_goat_pulse();
	}

	if(!(pulse % PULSE_MOBILE)) {
		mobile_activity();
	}

	if(!(pulse % PULSE_VIOLENCE)) {
		perform_violence();
	}

	if(!(pulse % (SECS_PER_MUD_HOUR * PASSES_PER_SEC))) {
		weather_and_time(1);
		affect_update();
		point_update();

	}

	if(auto_save && !(pulse % PULSE_AUTOSAVE)) {	/* 1 minute */
		if(++mins_since_crashsave >= autosave_time) {
			mins_since_crashsave = 0;
			Crash_save_all();
			House_save_all();
		}
	}

	if(!(pulse % PULSE_REMINDERS_TICK())) {
		mods::players::friendly_reminders::send();
	}
	if(!(pulse % PULSE_EVENT_MESSAGES_TICK())) {
		mods::players::event_messages::send();
	}

	if(!(pulse % PULSE_TIMESAVE)) {
		save_mud_time(&time_info);
	}

	/* Every pulse! Don't want them to stink the place up... */
	extract_pending_chars();

	if(!(pulse % PULSE_CLEANUP_SCRIPTED_STEPS)) {
		mods::scripted_sequence_runner::cleanup_scripted_steps();
	}

	if(!(pulse % 3)) {
		mods::mobs::room_watching::heartbeat();
	}

}


/* ******************************************************************
 *  general utility stuff (for local use)                            *
 ****************************************************************** */

/*
 *  new code to calculate time differences, which works on systems
 *  for which tv_usec is unsigned (and thus comparisons for something
 *  being < 0 fail).  Based on code submitted by ss@sirocco.cup.hp.com.
 */

/*
 * code to return the time difference between a and b (a-b).
 * always returns a nonnegative value (floors at 0).
 */
void timediff(struct timeval *rslt, struct timeval *a, struct timeval *b) {
	if(a->tv_sec < b->tv_sec) {
		*rslt = null_time;
	} else if(a->tv_sec == b->tv_sec) {
		if(a->tv_usec < b->tv_usec) {
			*rslt = null_time;
		} else {
			rslt->tv_sec = 0;
			rslt->tv_usec = a->tv_usec - b->tv_usec;
		}
	} else {			/* a->tv_sec > b->tv_sec */
		rslt->tv_sec = a->tv_sec - b->tv_sec;

		if(a->tv_usec < b->tv_usec) {
			rslt->tv_usec = a->tv_usec + 1000000 - b->tv_usec;
			rslt->tv_sec--;
		} else {
			rslt->tv_usec = a->tv_usec - b->tv_usec;
		}
	}
}

/*
 * Add 2 time values.
 *
 * Patch sent by "d. hall" <dhall@OOI.NET> to fix 'static' usage.
 */
void timeadd(struct timeval *rslt, struct timeval *a, struct timeval *b) {
	rslt->tv_sec = a->tv_sec + b->tv_sec;
	rslt->tv_usec = a->tv_usec + b->tv_usec;

	while(rslt->tv_usec >= 1000000) {
		rslt->tv_usec -= 1000000;
		rslt->tv_sec++;
	}
}


void record_usage(void) {
	int sockets_connected = 0, sockets_playing = 0;

	for(auto& d : descriptor_list) {
		sockets_connected++;

		if(STATE(d) == CON_PLAYING) {
			sockets_playing++;
		}
	}

	log("nusage: %d sockets connected, %d sockets playing",
	    sockets_connected, sockets_playing);

#ifdef RUSAGE	/* Not RUSAGE_SELF because it doesn't guarantee prototype. */
	{
		struct rusage ru;

		getrusage(RUSAGE_SELF, &ru);
		log("rusage: user time: %ld sec, system time: %ld sec, max res size: %ld",
		    ru.ru_utime.tv_sec, ru.ru_stime.tv_sec, ru.ru_maxrss);
	}
#endif

}



/*
 * Turn off echoing (specific to telnet client)
 */
void echo_off(mods::descriptor_data& d) {
	char off_string[] = {
		(char) IAC,
		(char) WILL,
		(char) TELOPT_ECHO,
		(char) 0,
	};

	write_to_output(d, "%s", off_string);
}

/*
 * Turn on echoing (specific to telnet client)
 */
void echo_on(mods::descriptor_data& d) {
	char on_string[] = {
		(char) IAC,
		(char) WONT,
		(char) TELOPT_ECHO,
		(char) 0
	};

	write_to_output(d, "%s", on_string);
}

void send_prompt(player_ptr_t& player) {
	if(player->is_npc()) {
		return;
	}
	if(player->paging()) {
		return;
	}
	auto& a = player->get_affect_dissolver();
	std::string hp = CAT(player->hp(),"H "),
	            mana = CAT(player->mana(),"M "),
	            move = CAT(player->move(),"V "),
	            fighting = "",
	            arrow = "{grn}> {/grn}",
	            start = "",
	            ammo = "",
	            affects = "";
	if(player->hp() < (player->max_hp() * 0.01)) {
		hp = CAT("{red}",player->hp(),"{/red}H ");
	}
	if(player->mana() < (player->max_mana() * 0.01)) {
		mana = CAT("{red}",player->mana(),"{/red}M ");
	}
	if(player->move() < (player->max_move() * 0.01)) {
		move = CAT("{red}",player->move(),"{/red}V ");
	}
	if(player->ghost() && player->ghost()->is_dissipated()) {
		start = "{yel}(:INVIS:){/yel}";
	}
	if(player->fighting()) {
		fighting = CAT("{red}[{/red}",mods::util::strip_color(player->fighting()->name()),"{red}]{/red}");
	}
	if(a.has_affect(mods::affects::affect_t::BLIND)) {
		affects += "BLIND";
	}
	if(a.has_affect(mods::affects::affect_t::DISORIENT)) {
		if(affects.length()) {
			affects += "|";
		}
		affects += "DISORIENT";
	}
	if(a.has_affect(mods::affects::affect_t::POISON)) {
		if(affects.length()) {
			affects += "|";
		}
		affects += "POISON";
	}
	if(a.has_affect(mods::affects::affect_t::INTIMIDATED)) {
		if(affects.length()) {
			affects += "|";
		}
		affects += "INTIMIDATED";
	}
	if(a.has_affect(mods::affects::affect_t::SCANNED)) {
		if(affects.length()) {
			affects += "|";
		}
		affects += "SCANNED";
	}
	if(a.has_affect(mods::affects::affect_t::TRACKED)) {
		if(affects.length()) {
			affects += "|";
		}
		affects += "TRACKED";
	}
	if(a.has_affect(mods::affects::affect_t::CORRODE)) {
		if(affects.length()) {
			affects += "|";
		}
		affects += "CORRODE";
	}
	if(a.has_affect(mods::affects::affect_t::BLEED)) {
		if(affects.length()) {
			affects += "|";
		}
		affects += "BLEED";
	}
	if(a.has_affect(mods::affects::affect_t::SLOWED)) {
		if(affects.length()) {
			affects += "|";
		}
		affects += "SLOWED";
	}
	if(player->primary() && player->primary()->has_rifle()) {
		auto left = player->primary()->rifle_instance->ammo;
		auto clip = player->primary()->rifle()->attributes->clip_size;
		if(left == 0) {
			ammo = CAT("{yel}[{red}RELOAD{/red}{yel}]{/yel}");
		} else {
			ammo = CAT("{yel}[",left,"/",clip,"]{/yel}");
		}
	}
	player->sendx(CAT(affects,"\r\n{grn}",player->name(),"::{/grn}",start,hp,mana,move,fighting,ammo,arrow));
}


void	write_to_q(std::string_view txt, mods::descriptor_data& d, int aliased) {
#ifdef __MENTOC_OUTPUT_DEBUGGUNG__
	std::cerr << "write_to_q:'" << txt.data() << "'\n";
#endif
	d.input.emplace_back(txt,aliased);
}

int get_from_q(mods::descriptor_data& d, char *dest, int *aliased) {
	if(d.input.size() == 0) {
		return 0;
	}
	strncpy(dest, d.input.begin()->text.c_str(), MAX_INPUT_LENGTH -1);
	*aliased = d.input.begin()->aliased;
	d.input.pop_front();
	return (1);
}




/* Add a new string to a player's output queue. For outside use. */
size_t write_to_output(mods::descriptor_data& t, const char *txt, ...) {
#ifdef __MENTOC_OUTPUT_DEBUGGUNG__
	std::cerr << "write_to_output:'" << txt << "'\n";
#endif
	va_list args;
	size_t left;

	va_start(args, txt);
	left = vwrite_to_output(t, txt, args);
	va_end(args);

	return left;
}

size_t vwrite_to_output(mods::descriptor_data& t, const char *format, va_list args) {
	if(!format || format[0] == '\0') {
		return 0;
	}
	static constexpr int txt_buffer_size_total = MAX_STRING_LENGTH;
	static constexpr int txt_buffer_size_allowable = txt_buffer_size_total - 12;
	std::array<char,txt_buffer_size_total> txt;
	std::fill(txt.begin(),txt.end(),0);

	int size;

	size = vsnprintf(&txt[0], txt_buffer_size_allowable, format, args);
	if(size == 0) {
		return 0;
	}

	/* If exceeding the size of the buffer, truncate it for the overflow message */
	if(size < 0) {
		size = txt_buffer_size_allowable;
		strncpy(&txt[0] + size - strlen(text_overflow), text_overflow,txt_buffer_size_allowable);
	} else {
		txt[std::min(size,txt_buffer_size_allowable)] = '\0';
	}
	t.queue_output(&txt[0],0,0);
#ifdef __MENTOC_OUTPUT_DEBUGGUNG__
	std::cerr << t.character->player.name.c_str() << "->vwrite_to_output'" << &txt[0] << "'\n";
#endif
	return 0;
}



/* ******************************************************************
 *  socket handling                                                  *
 ****************************************************************** */


/*
 * get_bind_addr: Return a struct in_addr that should be used in our
 * call to bind().  If the user has specified a desired binding
 * address, we try to bind to it; otherwise, we bind to INADDR_ANY.
 * Note that inet_aton() is preferred over inet_addr() so we use it if
 * we can.  If neither is available, we always bind to INADDR_ANY.
 */

struct in_addr *get_bind_addr() {
	static struct in_addr bind_addr;

	/* Clear the structure */
	memset((char *) &bind_addr, 0, sizeof(bind_addr));

	/* If DLFT_IP is unspecified, use INADDR_ANY */
	if(DFLT_IP == NULL) {
		bind_addr.s_addr = htonl(INADDR_ANY);
	} else {
		/* If the parsing fails, use INADDR_ANY */
		if(!parse_ip(DFLT_IP, &bind_addr)) {
			log("SYSERR: DFLT_IP of %s appears to be an invalid IP address",DFLT_IP);
			bind_addr.s_addr = htonl(INADDR_ANY);
		}
	}

	/* Put the address that we've finally decided on into the logs */
	if(bind_addr.s_addr == htonl(INADDR_ANY)) {
		log("Binding to all IP interfaces on this host.");
	} else {
		log("Binding only to IP address %s", inet_ntoa(bind_addr));
	}

	return (&bind_addr);
}

#ifdef HAVE_INET_ATON

/*
 * inet_aton's interface is the same as parse_ip's: 0 on failure, non-0 if
 * successful
 */
int parse_ip(const char *addr, struct in_addr *inaddr) {
	return (inet_aton(addr, inaddr));
}

#elif HAVE_INET_ADDR

/* inet_addr has a different interface, so we emulate inet_aton's */
int parse_ip(const char *addr, struct in_addr *inaddr) {
	long ip;

	if((ip = inet_addr(addr)) == -1) {
		return (0);
	} else {
		inaddr->s_addr = (unsigned long) ip;
		return (1);
	}
}

#else

/* If you have neither function - sorry, you can't do specific binding. */
int parse_ip(const char *addr, struct in_addr *inaddr) {
	log("SYSERR: warning: you're trying to set DFLT_IP but your system has no "
	    "functions to parse IP addresses (how bizarre!)");
	return (0);
}

#endif /* INET_ATON and INET_ADDR */



/* Sets the kernel's send buffer size for the descriptor */
int set_sendbuf(socket_t s) {
#if defined(SO_SNDBUF) && !defined(CIRCLE_MACINTOSH)
	int opt = MAX_SOCK_BUF;

	if(setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char *) &opt, sizeof(opt)) < 0) {
		perror("SYSERR: setsockopt SNDBUF");
		return (-1);
	}

#if 0

	if(setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char *) &opt, sizeof(opt)) < 0) {
		perror("SYSERR: setsockopt RCVBUF");
		return (-1);
	}

#endif

#endif

	return (0);
}

/** TODO: in order to fix the zero-socket issue, we
 * need to get a list of all the sockets that epoll_ctl is
 * monitoring. We then need to cross-reference those with
 * the currently active sockets and remove the ones
 * that aren't active
 */
void destroy_socket(socket_t sock_fd) {
	log("[%d] removing socket from epoll",sock_fd);
	auto r = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sock_fd, nullptr);
	close(sock_fd);
	if(r == -1) {
		logstrerror("SYSERR: destroy_player::epoll_ctl[error]->'", errno);
	}
}

int destroy_player(player_ptr_t&& player) {
	char_from_room(player);
	auto pl_iterator = std::find(mods::globals::player_list.begin(),
	        mods::globals::player_list.end(),
	        player);
	destroy_socket(player->socket());
	bool removed = false;
	{
		auto pmap_it = mods::globals::player_map.find(player->uuid());
		if(pmap_it != mods::globals::player_map.end()) {
			mods::globals::player_map.erase(pmap_it->first);
		}
	}
	{
		auto chmap_it = mods::globals::player_chmap.find(player->cd());
		if(chmap_it != mods::globals::player_chmap.end()) {
			mods::globals::player_chmap.erase(chmap_it->first);
		}
	}

	do {
		removed = false;
		for(auto it = mods::globals::socket_map.begin() ;
		    it != mods::globals::socket_map.end(); ++it) {
			auto name = player->name();
			if(it->first == 0) {
				mods::globals::socket_map.erase(it->first);
				removed = true;
				break;
			}
			if(!it->second) {
				log("Removing player from socket_map it->second invalid");
				mods::globals::socket_map.erase(it->first);
				removed = true;
				break;
			}
			if(it->second == nullptr) {
				log("Removing player from socket_map it->second.get() == nullptr");
				mods::globals::socket_map.erase(it->first);
				removed = true;
				break;
			}
			if(name.length()) {
				auto it_name = it->second->name().c_str();
				if(name.compare(it_name) == 0) {
					log("Removing player from socket_map name match");
					mods::globals::socket_map.erase(it->first);
					removed = true;
					break;
				}
			}
			if(it->second == player) {
				log("Removing player from socket_map pointer match");
				mods::globals::socket_map.erase(it->first);
				removed = true;
				break;
			}
		}
	} while(removed);
	mods::replenish::destroy_player(player->uuid());

	if(pl_iterator == mods::globals::player_list.end()) {
		log("SYSERR: WARNING! destroy_player cannot find player pointer in player_list!");
	} else {
		if(pl_iterator->use_count()) {
			log("Freeing player [%s] use_count[%d]",player->name().c_str(),pl_iterator->use_count());
			mods::globals::player_list.erase(pl_iterator);
		}
	}

	return 0;
}

int new_descriptor(socket_t s) {
	socket_t desc;
	int sockets_connected = 0;
	socklen_t i;
	static int last_desc = 0;	/* last descriptor number */
	struct sockaddr_in peer;
	struct hostent *from;

	/* accept the new connection */
	i = sizeof(peer);


	if((desc = accept(s, (struct sockaddr *) &peer, &i)) == INVALID_SOCKET) {
		perror("SYSERR: accept");
		destroy_socket(desc);//TODO: merge destroy_socket functionality into deregister_player process
		log("SYSERR: accept failed for socket");
		return MENTOC_INVALID_SOCKET;
	}

	/* keep it from blocking */
	nonblock(desc);

	/* set the send buffer size */
	if(set_sendbuf(desc) < 0) {
		log("SYSERR: set_sendbuf failed");
		destroy_socket(desc);
		return MENTOC_SEND_BUF_FAILED;
	}

	if(++sockets_connected >= max_players) {
		write_to_descriptor(desc, "Sorry, CircleMUD is full right now... please try again later!\r\n");
		log("Rejected user due to full server");
		destroy_socket(desc);
		return MENTOC_FULL_SERVER;
	}

	/** player_uuid() called in constructor */
	auto player = new_player();
	player->set_socket(desc);
	/* find the sitename */
	from = gethostbyaddr((char *) &peer.sin_addr, sizeof(peer.sin_addr), AF_INET);
	mods::message_server::new_connection(peer,from);
	player->set_ip(inet_ntoa(peer.sin_addr));
	if(mods::ban_system::ip::is_banned(peer)) {
		log("refusing new connection. User is banned by ip: %s",player->ip());
		mods::message_server::user_connection_rejected(peer,from);
		return MENTOC_PLAYER_IS_BANNED;
	}
	if(!from) {
		log("SYSERR: failed to get host by addr");
		/* find the numeric site address */
		player->set_host(inet_ntoa(peer.sin_addr));
	} else {
		player->set_host(from->h_name);
	}
	if(mods::ban_system::hostname::is_banned(player->host())) {
		log("refusing new connection. User is banned by hostname: %s, ip address: %s",player->host(), player->ip());
		mods::message_server::user_connection_rejected(peer,from);
		return MENTOC_PLAYER_IS_BANNED;
	}

	/* determine if the site is banned */
	/*
		 if(isbanned(player->host().c_str())) {
		 destroy_socket(desc);
		 mudlog(CMP, LVL_GOD, TRUE, "Connection attempt denied from [%s]", player->desc().host.c_str());
		 deregister_player(player.get());
		 return (0);
		 }else{
		 */
	log("new connection");

	/* initialize descriptor data */
	player->desc().login_time = time(0);
	player->desc().has_prompt = 1;  /* prompt is part of greetings */
	player->set_state(CON_GET_NAME);
	player->set_socket(desc);



	/*
	 * This isn't exactly optimal but allows us to make a design choice.
	 * Do we embed the history in mods::descriptor_data or keep it dynamically
	 * allocated and allow a user defined history size?
	 */
	if(++last_desc == 1000) {
		/* Ring buffer logic, it seems */
		last_desc = 1;
	}
	player->desc().desc_num = last_desc;


	GREETINGS = "Username:";
	write_to_output(player->desc(), "%s",GREETINGS.c_str());
	mods::globals::register_player(player);
	mods::globals::socket_map.insert(
	    std::pair<int,player_ptr_t>(
	        desc,std::move(player)
	    )
	);
	return (desc);
	//}
}


/*
 * Send all of the output that we've accumulated for a player out to
 * the player's descriptor.
 *
 * 32 byte GARBAGE_SPACE in MAX_SOCK_BUF used for:
 *	 2 bytes: prepended \r\n
 *	14 bytes: overflow message
 *	 2 bytes: extra \r\n for non-comapct
 *      14 bytes: unused
 */
int process_output(mods::descriptor_data& in_t) {
	log("[deprecated] process_output");
	return 0;
}


/*
 * perform_socket_write: takes a descriptor, a pointer to text, and a
 * text length, and tries once to send that text to the OS.  This is
 * where we stuff all the platform-dependent stuff that used to be
 * ugly #ifdef's in write_to_descriptor().
 *
 * This function must return:
 *
 * -1  If a fatal error was encountered in writing to the descriptor.
 *  0  If a transient failure was encountered (e.g. socket buffer full).
 * >0  To indicate the number of bytes successfully written, possibly
 *     fewer than the number the caller requested be written.
 *
 * Right now there are two versions of this function: one for Windows,
 * and one for all other platforms.
 */

#if defined(CIRCLE_ACORN)
	#define write	socketwrite
#endif

/* perform_socket_write for all Non-Windows platforms */
ssize_t perform_socket_write(socket_t desc, const char *txt, size_t length) {
	ssize_t result;

	result = write(desc, txt, length);

	if(result > 0) {
		/* Write was successful. */
		return (result);
	}

	if(result == 0) {
		/* This should never happen! */
		log("SYSERR: Huh??  write() returned 0???  Please report this!");
		return (-1);
	}

	/*
	 * result < 0, so an error was encountered - is it transient?
	 * Unfortunately, different systems use different constants to
	 * indicate this.
	 */

#ifdef EAGAIN		/* POSIX */

	if(errno == EAGAIN) {
		return (0);
	}

#endif

#ifdef EWOULDBLOCK	/* BSD */

	if(errno == EWOULDBLOCK) {
		return (0);
	}

#endif

	/* Looks like the error was fatal.  Too bad. */
	return (-1);
}


/*
 * write_to_descriptor takes a descriptor, and text to write to the
 * descriptor.  It keeps calling the system-level write() until all
 * the text has been delivered to the OS, or until an error is
 * encountered.
 *
 * Returns:
 * >=0  If all is well and good.
 *  -1  If an error was encountered, so that the player should be cut off.
 */
int write_to_descriptor(socket_t desc, const char *txt) {
	ssize_t bytes_written;
	size_t total = strlen(txt), write_total = 0;

	while(total > 0) {
		bytes_written = perform_socket_write(desc, txt, total);

		if(bytes_written < 0) {
			/* Fatal error.  Disconnect the player. */
			perror("SYSERR: Write to socket");
			return (-1);
		} else if(bytes_written == 0) {
			/* Temporary failure -- socket buffer full. */
			return (write_total);
		} else {
			txt += bytes_written;
			total -= bytes_written;
			write_total += bytes_written;
		}
	}

	return (write_total);
}


/*
 * Same information about perform_socket_write applies here. I like
 * standards, there are so many of them. -gg 6/30/98
 */
ssize_t perform_socket_read(socket_t desc, char *read_point, size_t space_left) {
	ssize_t ret;

	if(desc == 0) {
		log("WARNING: perform_socket_read was fed socket_t value of zero!");
		return 0;
	}
#if defined(CIRCLE_ACORN)
	ret = recv(desc, read_point, space_left, MSG_DONTWAIT);
#elif defined(CIRCLE_WINDOWS)
	ret = recv(desc, read_point, space_left, 0);
#else
	ret = read(desc, read_point, space_left);
#endif

	/* Read was successful. */
	if(ret > 0) {
		return (ret);
	}

	/* read() returned 0, meaning we got an EOF. */
	if(ret == 0) {
		log("WARNING: EOF on socket read (connection broken by peer)");
		return (-1);
	}

	/*
	 * read returned a value < 0: there was an error
	 */


	if(errno == EINTR) {
		return (0);
	}


	if(errno == EAGAIN) {
		return (0);
	}


#ifdef EWOULDBLOCK	/* BSD */

	if(errno == EWOULDBLOCK) {
		return (0);
	}

#endif /* EWOULDBLOCK */


	if(errno == ECONNRESET) {
		return (-1);
	}

	/*
	 * We don't know what happened, cut them off. This qualifies for
	 * a SYSERR because we have no idea what happened at this point.
	 */
	perror("SYSERR: perform_socket_read: about to lose connection");
	return (-1);
}


/*
 * ASSUMPTION: There will be no newlines in the raw input buffer when this
 * function is called.  We must maintain that before returning.
 *
 * Ever wonder why 'tmp' had '+8' on it?  The crusty old code could write
 * MAX_INPUT_LENGTH+1 bytes to 'tmp' if there was a '$' as the final
 * character in the input buffer.  This would also cause 'space_left' to
 * drop to -1, which wasn't very happy in an unsigned variable.  Argh.
 * So to fix the above, 'tmp' lost the '+8' since it doesn't need it
 * and the code has been changed to reserve space by accepting one less
 * character. (Do you really need 256 characters on a line?)
 * -gg 1/21/2000
 */
int process_input(mods::descriptor_data& t) {
	int buf_length, failed_subst;
	ssize_t bytes_read;
	size_t space_left;
	char *ptr, *read_point, *write_point, *nl_pos = NULL;
	std::array<char,MAX_INPUT_LENGTH> tmp;

	std::fill(tmp.begin(),tmp.end(),0);

	/* first, find the point where we left off reading data */
	if(t.inbuf.length()) {
#ifdef __MENTOC_SHOW_COMM_CPP_DEBUG_OUTPUT__
		std::cerr << green_str("input length is:") << t.inbuf.length() << "\n";
#endif
		buf_length = t.inbuf.length();
		bcopy(t.inbuf.c_str(),&tmp[0],t.inbuf.length());
		read_point = static_cast<char*>(&tmp[buf_length-1]);
		space_left = MAX_RAW_INPUT_LENGTH - buf_length - 1;
	} else {
		read_point = &tmp[0];
		space_left = MAX_RAW_INPUT_LENGTH - 1;
	}

	do {
		if(space_left <= 0) {
			log("WARNING: process_input: about to close connection: input overflow");
			return (-1);
		}

		if(t.descriptor == 0) {
			return -2;
		}
		bytes_read = perform_socket_read(t.descriptor, read_point, space_left);
#ifdef __MENTOC_SHOW_SOCKET_READ_DEBUG_OUTPUT__
		std::cerr << "bytes_read:" << bytes_read << "\n";
		for(int ix = 0; ix < bytes_read; ++ix) {
			std::cerr << "byte[" << ix << "]:->" << std::hex << (int)read_point[ix] << "(dec:" << std::dec << (int)read_point[ix] << ")\n";
		}
		std::cerr << std::dec;
#endif

		if(bytes_read == 5) {
			int64_t a;
			memcpy(&a,(void*)read_point,sizeof(a));
			/** control+c */
			if(a == 30031213823) {
				return PROCESS_INPUT_CONTROL_C;
			}
		}
		/** control + d */
		if(bytes_read == 1 && (int)read_point[0] == 4) {
			return -1;
		}

		if(bytes_read < 0) {	/* Error, disconnect them. */
			return (-3);
		} else if(bytes_read == 0) {	/* Just blocking, no problems. */
			return (0);
		}

		/* at this point, we know we got some data from the read */
		*(read_point + bytes_read) = '\0';	/* terminate the string */

		/* search for a newline in the data we just read */
		for(ptr = read_point; *ptr && !nl_pos; ptr++)
			if(ISNEWL(*ptr)) {
				nl_pos = ptr;
			}


		read_point += bytes_read;
		space_left -= bytes_read;

		/*
		 * on some systems such as AIX, POSIX-standard nonblocking I/O is broken,
		 * causing the MUD to hang when it encounters input not terminated by a
		 * newline.  This was causing hangs at the Password: prompt, for example.
		 * I attempt to compensate by always returning after the _first_ read, instead
		 * of looping forever until a read returns -1.  This simulates non-blocking
		 * I/O because the result is we never call read unless we know from select()
		 * that data is ready (process_input is only called if select indicates that
		 * this descriptor is in the read set).  JE 2/23/95.
		 */
#if !defined(POSIX_NONBLOCK_BROKEN)
	} while(nl_pos == NULL);

#else
	}

	while(0);

	if(nl_pos == NULL) {
		return (0);
	}

#endif /* POSIX_NONBLOCK_BROKEN */

	/*
	 * okay, at this point we have at least one newline in the string; now we
	 * can copy the formatted data to a new array for further processing.
	 */
	read_point = &tmp[0];

	while(nl_pos != nullptr) {
		write_point = &tmp[0];
		space_left = MAX_INPUT_LENGTH - 1;

		/* The '> 1' reserves room for a '$ => $$' expansion. */
		for(ptr = read_point; (space_left > 1) && (ptr < nl_pos); ptr++) {
			if(*ptr == '\b' || *ptr == 127) {  /* handle backspacing or delete key */
				if(write_point > &tmp[0]) {
					if(*(--write_point) == '$') {
						write_point--;
						space_left += 2;
					} else {
						space_left++;
					}
				}
			} else if(isascii(*ptr) && isprint(*ptr)) {
				if((*(write_point++) = *ptr) == '$') {		/* copy one character */
					*(write_point++) = '$';	/* if it's a $, double it */
					space_left -= 2;
				} else {
					space_left--;
				}
			}
		}

		*write_point = '\0';

		if((space_left <= 0) && (ptr < nl_pos)) {
			char buffer[MAX_INPUT_LENGTH + 64];

			snprintf(buffer, sizeof(buffer), "Line too long.  Truncated to:\r\n%s\r\n", &tmp[0]);

			if(write_to_descriptor(t.descriptor, buffer) < 0) {
				return (-1);
			}
		}

		if(t.snoop_by) {
			write_to_output(*t.snoop_by, "%% %s\r\n", &tmp[0]);
		}

		failed_subst = 0;

		if(tmp[0] == '!' && !tmp[1]) {	/* Redo last command. */
			strcpy(&tmp[0], t.last_input.data());    /* strcpy: OK (by mutual MAX_INPUT_LENGTH) */
		} else if(tmp[0] == '!' && tmp[1]) {
			char *commandln = (&tmp[1]);
			skip_spaces(&commandln);

			for(auto& line : t.history) {
				if(is_abbrev(commandln, line)) {
					strcpy(&tmp[0], line);	/* strcpy: OK (by mutual MAX_INPUT_LENGTH) */
					t.last_input =  &tmp[0];	/* strcpy: OK (by mutual MAX_INPUT_LENGTH) */
					write_to_output(t, "%s\r\n", &tmp[0]);
					break;
				}
			}
		} else if(tmp[0] == '^') {
			if(!(failed_subst = perform_subst(t, t.last_input.data(), &tmp[0]))) {
				t.last_input = &tmp[0];    /* strcpy: OK (by mutual MAX_INPUT_LENGTH) */
			}
		} else {
			t.last_input =  &tmp[0];	/* strcpy: OK (by mutual MAX_INPUT_LENGTH) */

			/** !fixme: !mods |-> I think history should just be a std::array<std::string,HISTORY_SIZE> and we do ring buffer logic around it. !fixme: */
			if(t.history_pos < HISTORY_SIZE && t.history[t.history_pos].length()) {
				t.history[t.history_pos].clear();    /* Clear the old line. */
				t.history_pos = 0;
			}
			if(t.history_pos < HISTORY_SIZE) {
				t.history[t.history_pos].assign(&tmp[0]);	/* Save the new. */
			}

			if(++t.history_pos >= HISTORY_SIZE) {	/* Wrap to top. */
				t.history_pos = 0;
			}
		}

		if(!failed_subst) {
			write_to_q(&tmp[0], t, 0);
		}

		/* find the end of this line */
		while(ISNEWL(*nl_pos)) {
			nl_pos++;
		}

		/* see if there's another newline in the input buffer */
		read_point = ptr = nl_pos;

		for(nl_pos = NULL; *ptr && !nl_pos; ptr++)
			if(ISNEWL(*ptr)) {
				nl_pos = ptr;
			}
	}

	/* now move the rest of the buffer up to the beginning for the next pass */
	if(read_point) {
		t.inbuf = read_point;
	}

	return (1);
}



/* perform substitution for the '^..^' csh-esque syntax orig is the
 * orig string, i.e. the one being modified.  subst contains the
 * substition string, i.e. "^telm^tell"
 */
int perform_subst(mods::descriptor_data& t, char *orig, char *subst) {
	char newsub[MAX_INPUT_LENGTH + 5];

	char *first, *second, *strpos;

	/*
	 * first is the position of the beginning of the first string (the one
	 * to be replaced
	 */
	first = subst + 1;

	/* now find the second '^' */
	if(!(second = strchr(first, '^'))) {
		write_to_output(t, "Invalid substitution.\r\n");
		return (1);
	}

	/* terminate "first" at the position of the '^' and make 'second' point
	 * to the beginning of the second string */
	*(second++) = '\0';

	/* now, see if the contents of the first string appear in the original */
	if(!(strpos = strstr(orig, first))) {
		write_to_output(t, "Invalid substitution.\r\n");
		return (1);
	}

	/* now, we construct the new string for output. */

	/* first, everything in the original, up to the string to be replaced */
	strncpy(newsub, orig, strpos - orig);	/* strncpy: OK (newsub:MAX_INPUT_LENGTH+5 > orig:MAX_INPUT_LENGTH) */
	newsub[strpos - orig] = '\0';

	/* now, the replacement string */
	strncat(newsub, second, MAX_INPUT_LENGTH - strlen(newsub) - 1);	/* strncpy: OK */

	/* now, if there's anything left in the original after the string to
	 * replaced, copy that too. */
	if(((strpos - orig) + strlen(first)) < strlen(orig)) {
		strncat(newsub, strpos + strlen(first), MAX_INPUT_LENGTH - strlen(newsub) - 1);    /* strncpy: OK */
	}

	/* terminate the string in case of an overflow from strncat */
	newsub[MAX_INPUT_LENGTH - 1] = '\0';
	strcpy(subst, newsub);	/* strcpy: OK (by mutual MAX_INPUT_LENGTH) */

	return (0);
}



void close_socket(mods::descriptor_data& d) {
	/** !fixme: there are some free() calls here that need to be eliminated but first the mallocs need to be found and the members turned into stl containers. */
	d("erasing descriptor from socket map");
	mods::globals::socket_map.erase(d.descriptor);
	d("destroying socket desc");
	destroy_socket(d.descriptor);

	/* Forget snooping */
	if(d.snooping) {
		d("unsetting snoop_by");
		d.snooping->snoop_by = nullptr;
	}

	if(d.snoop_by) {
		d("unsetting snooping");
		write_to_output(*d.snoop_by, "Your victim is no longer among us.\r\n");
		d.snoop_by->snooping = nullptr;
	}

	if(d.character && d.character->desc) {
		log("d.character active");
		/* If we're switched, this resets the mobile taken. */
		d.character->desc->clear();

		/* Plug memory leak, from Eric Green. */
		//if(!IS_NPC(d.character) && PLR_FLAGGED(d.character, PLR_MAILING) && d.str) {
		//	if(*(d.str)) {
		//		free(*(d.str));
		//	}

		//	free(d.str);
		//}

		if(STATE(d)== CON_PLAYING || STATE(d)== CON_CLOSE ||  STATE(d)== CON_DISCONNECT) {
			char_data *link_challenged = d.original ? d.original : d.character;

			/* We are guaranteed to have a person. */
			act("$n has lost $s link.", TRUE, link_challenged, 0, 0, TO_ROOM);
			mods::db::save_char(std::make_shared<mods::player>(link_challenged));
			mudlog(NRM, MAX(LVL_IMMORT, GET_INVIS_LEV(link_challenged)), TRUE, "Closing link to: %s.", GET_NAME(link_challenged).c_str());
		} else {
			mudlog(CMP, LVL_IMMORT, TRUE, "Losing player: %s.", GET_NAME(d.character) ? GET_NAME(d.character).c_str() : "<null>");
		}
	} else {
		mudlog(CMP, LVL_IMMORT, TRUE, "Losing descriptor without char.");
	}

	/* JE 2/22/95 -- part of my unending quest to make switch stable */
	if(d.original && d.original->has_desc) {
		d.original->desc->clear();
	}

	if(d.showstr_head) {
		log("freeing showstr_head");
		free(d.showstr_head);
	}

	if(d.showstr_count) {
		log("freeing showstr_vector");
		free(d.showstr_vector);
	}
	/** !TODO: do we use deregister_player here? */
}



void check_idle_passwords(void) {
	for(auto& d : descriptor_list) {
		if(STATE(d) != CON_PASSWORD && STATE(d) != CON_GET_NAME) {
			continue;
		}

		if(!d.idle_tics) {
			d.idle_tics++;
			continue;
		} else {
			echo_on(d);
			write_to_output(d, "\r\nTimed out... goodbye.\r\n");
			STATE(d) = CON_CLOSE;
		}
	}
}



/*
 * I tried to universally convert Circle over to POSIX compliance, but
 * alas, some systems are still straggling behind and don't have all the
 * appropriate defines.  In particular, NeXT 2.x defines O_NDELAY but not
 * O_NONBLOCK.  Krusty old NeXT machines!  (Thanks to Michael Jones for
 * this and various other NeXT fixes.)
 */

#if defined(CIRCLE_WINDOWS)

void nonblock(socket_t s) {
	unsigned long val = 1;
	ioctlsocket(s, FIONBIO, &val);
}

#elif defined(CIRCLE_AMIGA)

void nonblock(socket_t s) {
	long val = 1;
	IoctlSocket(s, FIONBIO, &val);
}

#elif defined(CIRCLE_ACORN)

void nonblock(socket_t s) {
	int val = 1;
	socket_ioctl(s, FIONBIO, &val);
}

#elif defined(CIRCLE_VMS)

void nonblock(socket_t s) {
	int val = 1;

	if(ioctl(s, FIONBIO, &val) < 0) {
		perror("SYSERR: Fatal error executing nonblock (comm.c)");
		exit_with(1);
	}
}

#elif defined(CIRCLE_UNIX) || defined(CIRCLE_OS2) || defined(CIRCLE_MACINTOSH)

#ifndef O_NONBLOCK
	#define O_NONBLOCK O_NDELAY
#endif

void nonblock(socket_t s) {
	int flags;

	flags = fcntl(s, F_GETFL, 0);
	flags |= O_NONBLOCK;

	if(fcntl(s, F_SETFL, flags) < 0) {
		perror("SYSERR: Fatal error executing nonblock (comm.c)");
		exit_with(1);
	}
}

#endif  /* CIRCLE_UNIX || CIRCLE_OS2 || CIRCLE_MACINTOSH */


/* ******************************************************************
 *  signal-handling functions (formerly signals.c).  UNIX only.      *
 ****************************************************************** */

#if defined(CIRCLE_UNIX) || defined(CIRCLE_MACINTOSH)

RETSIGTYPE reread_wizlists(int sig) {
	reread_wizlist = TRUE;
}


RETSIGTYPE unrestrict_game(int sig) {
	emergency_unban = TRUE;
}

#ifdef CIRCLE_UNIX

/* clean up our zombie kids to avoid defunct processes */
RETSIGTYPE reap(int sig) {
	while(waitpid(-1, NULL, WNOHANG) > 0);

	my_signal(SIGCHLD, reap);
}

/* Dying anyway... */
RETSIGTYPE checkpointing(int sig) {
	if(!tics) {
		log("SYSERR: CHECKPOINT shutdown: tics not updated. (Infinite loop suspected)");
		abort();
	} else {
		tics = 0;
	}
}


/* Dying anyway... */
RETSIGTYPE hupsig(int sig) {
	log("SYSERR: Received SIGHUP, SIGINT, or SIGTERM.  Shutting down...");
	exit_with(1);			/* perhaps something more elegant should
								 * substituted */
}

#endif	/* CIRCLE_UNIX */

/*
 * This is an implementation of signal() using sigaction() for portability.
 * (sigaction() is POSIX; signal() is not.)  Taken from Stevens' _Advanced
 * Programming in the UNIX Environment_.  We are specifying that all system
 * calls _not_ be automatically restarted for uniformity, because BSD systems
 * do not restart select(), even if SA_RESTART is used.
 *
 * Note that NeXT 2.x is not POSIX and does not have sigaction; therefore,
 * I just define it to be the old signal.  If your system doesn't have
 * sigaction either, you can use the same fix.
 *
 * SunOS Release 4.0.2 (sun386) needs this too, according to Tim Aldric.
 */

#ifndef POSIX
#define my_signal(signo, func) signal(signo, func)
#else
sigfunc *my_signal(int signo, sigfunc *func) {
	struct sigaction sact, oact;

	sact.sa_handler = func;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags = 0;
#ifdef SA_INTERRUPT
	sact.sa_flags |= SA_INTERRUPT;	/* SunOS */
#endif

	if(sigaction(signo, &sact, &oact) < 0) {
		return (SIG_ERR);
	}

	return (oact.sa_handler);
}
#endif				/* POSIX */


void signal_setup(void) {
#ifndef CIRCLE_MACINTOSH
	itimerval itime;
	timeval interval;

	/* user signal 1: reread wizlists.  Used by autowiz system. */
	my_signal(SIGUSR1, reread_wizlists);

	/*
	 * user signal 2: unrestrict game.  Used for emergencies if you lock
	 * yourself out of the MUD somehow.  (Duh...)
	 */
	my_signal(SIGUSR2, unrestrict_game);

	/*
	 * set up the deadlock-protection so that the MUD aborts itself if it gets
	 * caught in an infinite loop for more than 3 minutes.
	 */
	interval.tv_sec = 180;
	interval.tv_usec = 0;
	itime.it_interval = interval;
	itime.it_value = interval;
	setitimer(ITIMER_VIRTUAL, &itime, NULL);
	my_signal(SIGVTALRM, checkpointing);

	/* just to be on the safe side: */
	my_signal(SIGHUP, hupsig);
	my_signal(SIGCHLD, reap);
#endif /* CIRCLE_MACINTOSH */
	my_signal(SIGINT, hupsig);
	my_signal(SIGTERM, hupsig);
	my_signal(SIGPIPE, SIG_IGN);
	my_signal(SIGALRM, SIG_IGN);
}

#endif	/* CIRCLE_UNIX || CIRCLE_MACINTOSH */

/* ****************************************************************
 *       Public routines for system-to-player-communication        *
 **************************************************************** */

size_t send_to_char(char_data *ch, const char *messg, ...) {
	if(ch->has_desc && messg && *messg) {
		size_t left;
		va_list args;
		va_start(args, messg);
		if(ch->desc->has_prompt) {
			vwrite_to_output(*ch->desc,"\r\n",args);
		}
		left = vwrite_to_output(*ch->desc, messg, args);
		va_end(args);
		return left;
	}

	return 0;
}

void send_to_all(const char *messg, ...) {
	va_list args;

	if(messg == NULL) {
		return;
	}

#ifdef __MENTOC_OUTPUT_DEBUGGUNG__
	std::cerr << "send_to_all:'" << messg << "'\n";
#endif
	va_start(args, messg);
	for(auto& i : descriptor_list) {
		if(STATE(i) != CON_PLAYING) {
			continue;
		}
		if(i.has_prompt) {
			vwrite_to_output(i,"\r\n",args);
		}

		vwrite_to_output(i, messg, args);
	}
	va_end(args);
}


void send_to_outdoor(const char *messg, ...) {
	if(!messg || !*messg) {
		return;
	}

#ifdef __MENTOC_OUTPUT_DEBUGGUNG__
	std::cerr << "send_to_outdoor:'" << messg << "'\n";
#endif
	va_list args;
	va_start(args, messg);
	for(auto& i : descriptor_list) {

		if(STATE(i) != CON_PLAYING || i.character == nullptr) {
			continue;
		}

		if(!AWAKE(i.character) || !OUTSIDE(i.character)) {
			continue;
		}

		if(i.has_prompt) {
			vwrite_to_output(i,"\r\n",args);
		}
		vwrite_to_output(i, messg, args);
	}
	va_end(args);
}

void send_to_room_except(room_rnum room, const std::vector<player_ptr_t>& except, const char *messg, ...) {
#ifdef __MENTOC_OUTPUT_DEBUGGUNG__
	std::cerr << "send_to_room_except(0):'" << messg << "'\n";
#endif
	va_list args;

	if(messg == NULL) {
		return;
	}

	bool send = true;
	va_start(args, messg);
	for(auto& player :  mods::globals::get_room_list(room)) {
		send = true;
		for(const auto& e : except) {
			if(player->uuid() == e->uuid()) {
				send = false;
				break;
			}
		}
		if(send) {
			player->sendln(messg);//, args);
		}
	}
	va_end(args);
}
void send_to_room_except(room_rnum room, std::vector<uuid_t> except, const char *messg, ...) {
#ifdef __MENTOC_OUTPUT_DEBUGGUNG__
	std::cerr << "send_to_room_except(1):'" << messg << "'\n";
#endif
	va_list args;

	if(messg == NULL) {
		return;
	}
	bool emplace = true;
	va_start(args, messg);
	for(auto& player :  mods::globals::get_room_list(room)) {
		emplace = true;
		for(const auto& e : except) {
			if(player->uuid() == e) {
				emplace = false;
				break;
			}
		}
		if(emplace) {
			player->sendln(messg);//, args);
		}
	}
	va_end(args);
}


void send_to_room_except(room_rnum room, const std::vector<char_data*>& except, const char *messg, ...) {
#ifdef __MENTOC_OUTPUT_DEBUGGUNG__
	std::cerr << "send_to_room_except(2):'" << messg << "'\n";
#endif
	va_list args;

	if(messg == NULL) {
		return;
	}

	va_start(args, messg);
	for(auto& p : mods::globals::get_room_list(room)) {
		if(std::find(except.begin(),except.end(),p->cd()) == except.end()) {
			p->sendln(messg);//, args);
		}
	}
	va_end(args);
}

void send_to_room_except(room_rnum room, player_ptr_t except_me, const char *messg, ...) {
#ifdef __MENTOC_OUTPUT_DEBUGGUNG__
	std::cerr << "send_to_room_except(3):'" << messg << "'\n";
#endif
	va_list args;

	if(messg == NULL) {
		return;
	}

	const auto& u = except_me->uuid();
	va_start(args, messg);
	for(auto& p : mods::globals::get_room_list(room)) {
		if(u != p->uuid()) {
			p->sendln(messg);//, args);
		}
	}
	va_end(args);
}


void send_to_room(room_rnum room, const char *messg, ...) {
#ifdef __MENTOC_OUTPUT_DEBUGGUNG__
	std::cerr << "send_to_room:'" << messg << "'\n";
#endif
	va_list args;

	if(messg == NULL) {
		return;
	}

	va_start(args, messg);
	for(auto& p : mods::globals::get_room_list(room)) {
		p->sendln(messg);//,args);
	}
	va_end(args);
}



const char *ACTNULL = "<NULL>";

#define CHECK_NULL(pointer, expression) \
	if ((pointer) == NULL) i = ACTNULL; else i = (expression);


/* higher-level communication: the act() function */
void perform_act(const char *orig, char_data *ch, obj_data *obj,
    const void *vict_obj, char_data *to) {
	char lbuf[MAX_STRING_LENGTH], *buf, *j;
	bool uppercasenext = FALSE;

	std::string i = "";

	buf = lbuf;

	for(;;) {
		if(*orig == '$') {
			switch(*(++orig)) {
				case 'n':
					/**
					 * Notes:
					 *  'to'-> this is the player in the room
					 *  'ch' -> this is the mob or player that performed the action
					 *  PERS(ch,to) will resolve to something like:
					 *  	"$n leaves north."
					 *
					 */
					i = mods::string(PERS(ch, to)).str();
					break;

				case 'N':
					CHECK_NULL(vict_obj, mods::string(PERS((const char_data *) vict_obj, to)).str());
					break;

				case 'm':
					i = mods::string(HMHR(ch)).str();
					break;

				case 'M':
					CHECK_NULL(vict_obj, mods::string(HMHR((const char_data *) vict_obj)).str());
					break;

				case 's':
					i = mods::string(HSHR(ch)).str();
					break;

				case 'S':
					CHECK_NULL(vict_obj, mods::string(HSHR((const char_data *) vict_obj)).str());
					break;

				case 'e':
					i = mods::string(HSSH(ch)).str();
					break;

				case 'E':
					CHECK_NULL(vict_obj, mods::string(HSSH((const char_data *) vict_obj)).str());
					break;
				case 'f': {
						if(ch && to && ch->char_specials.fighting == to) {
							i = "{red}[!]{/red}";
						} else {
							i = "";
						}
					}
					break;

				case 'o':
					CHECK_NULL(obj, mods::string(OBJN(obj, to)).str());
					break;

				case 'O':
					CHECK_NULL(vict_obj, mods::string(OBJN((const struct obj_data *) vict_obj, to)).str());
					break;

				case 'p':
					CHECK_NULL(obj, mods::string(OBJS(obj, to)).str());
					break;

				case 'P':
					CHECK_NULL(vict_obj, mods::string(OBJS((const struct obj_data *) vict_obj, to)).str());
					break;

				case 'a':
					CHECK_NULL(obj, mods::string(SANA(obj)).str());
					break;

				case 'A':
					CHECK_NULL(vict_obj, mods::string(SANA((const struct obj_data *) vict_obj)).str());
					break;

				case 'T':
					CHECK_NULL(vict_obj, (const char *) vict_obj);
					break;

				case 'F':
					CHECK_NULL(vict_obj, mods::string(fname((const char *) vict_obj)).str());
					break;

				/* uppercase previous word */
				case 'u':
					for(j=buf; j > lbuf && !isspace((int) *(j-1)); j--);

					if(j != buf) {
						*j = UPPER(*j);
					}

					i = "";
					break;

				/* uppercase next word */
				case 'U':
					uppercasenext = TRUE;
					i = "";
					break;

				case '$':
					i = "$";
					break;

				default:
					log("SYSERR: Illegal $-code to act(): %c", *orig);
					log("SYSERR: %s", orig);
					i = "";
					break;
			}
			for(const auto& ch : i) {
				*buf = ch;
				if(uppercasenext && !isspace((int) *buf)) {
					*buf = UPPER(*buf);
					uppercasenext = FALSE;
				}
				buf++;
			}

			orig++;
		} else if(!(*(buf++) = *(orig++))) {
			break;
		} else if(uppercasenext && !isspace((int) *(buf-1))) {
			*(buf-1) = UPPER(*(buf-1));
			uppercasenext = FALSE;
		}
	}

	*(--buf) = '\0';
	ptr(to)->sendln(CAP(lbuf));
}


#define SENDOK(ch)	((ch)->has_desc && (to_sleeping || AWAKE(ch)) && \
    (IS_NPC(ch) || !PLR_FLAGGED((ch), PLR_WRITING)))

void act(const std::string& str, int hide_invisible, char_data *ch,
    obj_data *obj, void *vict_obj, int type) {
	int to_sleeping;

	if(str.length() == 0) {
		return;
	}

	/*
	 * Warning: the following TO_SLEEP code is a hack.
	 *
	 * I wanted to be able to tell act to deliver a message regardless of sleep
	 * without adding an additional argument.  TO_SLEEP is 128 (a single bit
	 * high up).  It's ONLY legal to combine TO_SLEEP with one other TO_x
	 * command.  It's not legal to combine TO_x's with each other otherwise.
	 * TO_SLEEP only works because its value "happens to be" a single bit;
	 * do not change it to something else.  In short, it is a hack.
	 */

	/* check if TO_SLEEP is there, and remove it if it is. */
	if((to_sleeping = (type & TO_SLEEP))) {
		type &= ~TO_SLEEP;
	}

	if(type == TO_CHAR) {
		if(ch && SENDOK(ch)) {
			perform_act(str.c_str(), ch, obj, vict_obj, ch);
		}

		return;
	}

	if(type == TO_VICT) {
		auto vict = static_cast<char_data*>(vict_obj);
		if(vict != nullptr && SENDOK(vict)) {
			perform_act(str.c_str(), ch, obj, vict_obj, vict);
		}

		return;
	}

	/* ASSUMPTION: at this point we know type must be TO_NOTVICT or TO_ROOM */

	if(ch && IN_ROOM(ch) != NOWHERE) {
		//to = world[IN_ROOM(ch)].people;
	} else if(obj != 0 && IN_ROOM(obj) != NOWHERE) {
		//to = world[IN_ROOM(obj)].people;
	} else {
		log("SYSERR: no valid target to act()!");
		return;
	}

	for(auto& to_player : mods::globals::get_room_list(IN_ROOM(ch))) {
		if(!to_player || to_player->authenticated() == false) {
			continue;
		}
		auto to = to_player->cd();
		if(!SENDOK(to) || (to == ch)) {
			continue;
		}

		if(hide_invisible && ch && !CAN_SEE(to, ch)) {
			continue;
		}

		if(type != TO_ROOM && to == vict_obj) {
			continue;
		}

		perform_act(str.c_str(), ch, obj, vict_obj, to);
	}
}


/* Prefer the file over the descriptor. */
void setup_log(const char *filename, int fd) {
	FILE *s_fp;

#if defined(__MWERKS__) || defined(__GNUC__)
	s_fp = stderr;
#else

	if((s_fp = fdopen(STDERR_FILENO, "w")) == NULL) {
		log("SYSERR: Error opening stderr, trying stdout.");

		if((s_fp = fdopen(STDOUT_FILENO, "w")) == NULL) {
			log("SYSERR: Error opening stdout, trying a file.");

			/* If we don't have a file, try a default. */
			if(filename == NULL || *filename == '\0') {
				filename = "log/syslog";
			}
		}
	}

#endif

	if(filename == NULL || *filename == '\0') {
		/* No filename, set us up with the descriptor we just opened. */
		logfile = s_fp;
		log("Using file descriptor for logging.");
		return;
	}

	/* We honor the default filename first. */
	if(open_logfile(filename, s_fp)) {
		return;
	}

	/* Well, that failed but we want it logged to a file so try a default. */
	if(open_logfile("log/syslog", s_fp)) {
		return;
	}

	/* Ok, one last shot at a file. */
	if(open_logfile("syslog", s_fp)) {
		return;
	}

	/* Erp, that didn't work either, just die. */
	log("SYSERR: Couldn't open anything to log to, giving up.");
	exit_with(1);
}

int open_logfile(const char *filename, FILE *stderr_fp) {
	if(stderr_fp) {	/* freopen() the descriptor. */
		logfile = freopen(filename, "w", stderr_fp);
	} else {
		logfile = fopen(filename, "w");
	}

	if(logfile) {
		printf("Using log file '%s'%s.\n",
		    filename, stderr_fp ? " with redirection" : "");
		return (TRUE);
	}

	return (FALSE);
}

/*
 * This may not be pretty but it keeps game_loop() neater than if it was inline.
 */
void circle_sleep(struct timeval *timeout) {
#ifdef __MENTOC_CLAMP_CIRCLE_SLEEP_VALUES__
	if(timeout->tv_usec > 100000) {
		std::cerr << "clamp: " << timeout->tv_usec << "\n";
		timeout->tv_usec = 100000;
	}
	if(timeout->tv_sec) {
		std::cerr << "tv_sec:'" << timeout->tv_sec << "'|";
		sleep(timeout->tv_sec);
	}
	if(timeout->tv_usec < 99999) {
		std::cerr << "tv_usec:'" << timeout->tv_usec << "'\n";
	}
	usleep(timeout->tv_usec);
#else
	sleep(timeout->tv_sec);
	usleep(timeout->tv_usec);
#endif
}
