/* ************************************************************************
*   File: comm.h                                        Part of CircleMUD *
*  Usage: header file: prototypes of public communication functions       *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#define NUM_RESERVED_DESCS	8
#include "structs.h"
/* comm.c */
RETSIGTYPE reread_wizlists(int sig);
RETSIGTYPE unrestrict_game(int sig);
RETSIGTYPE reap(int sig);
RETSIGTYPE checkpointing(int sig);
RETSIGTYPE hupsig(int sig);
ssize_t perform_socket_read(socket_t desc, char *read_point,size_t space_left);
ssize_t perform_socket_write(socket_t desc, const char *txt,size_t length);
void echo_off(mods::descriptor_data &d);
void echo_on(mods::descriptor_data &d);
void circle_sleep(struct timeval *timeout);
//int get_from_q(struct txt_q *queue, char *dest, int *aliased);
int get_from_q(mods::descriptor_data& d, char *dest, int *aliased);
void init_game(ush_int port);
void signal_setup(void);
void game_loop(socket_t mother_desc);
socket_t init_socket(ush_int port);
int new_descriptor(socket_t s);
int get_max_players(void);
int process_output(mods::descriptor_data &in_t);
int process_input(mods::descriptor_data &t);
void timediff(struct timeval *diff, struct timeval *a, struct timeval *b);
void timeadd(struct timeval *sum, struct timeval *a, struct timeval *b);
void flush_queues(mods::descriptor_data &d);
void nonblock(socket_t s);
int perform_subst(mods::descriptor_data &t, char *orig, char *subst);
void record_usage(void);
char *make_prompt(mods::descriptor_data &point);
void check_idle_passwords(void);
void heartbeat(int pulse);
struct in_addr *get_bind_addr(void);
int parse_ip(const char *addr, struct in_addr *inaddr);
int set_sendbuf(socket_t s);
void setup_log(const char *filename, int fd);
int open_logfile(const char *filename, FILE *stderr_fp);
size_t	send_to_char(char_data *ch, const char *messg, ...) __attribute__((format(printf, 2, 3)));
void write_to_char(char_data* ch, std::string_view msg,bool newline, bool plain);
void	send_to_all(const char *messg, ...) __attribute__((format(printf, 1, 2)));
void	send_to_room(room_rnum room, const char *messg, ...) __attribute__((format(printf, 2, 3)));
void send_to_room_except(room_rnum, const std::vector<char_data*>&, const char *, ...);
void send_to_room_except(room_rnum room, player_ptr_t except_me, const char *messg, ...);
void send_to_room_except(room_rnum room, const std::vector<player_ptr_t>& except_me, const char *messg, ...);
void	send_to_outdoor(const char *messg, ...) __attribute__((format(printf, 1, 2)));
void	close_socket(mods::descriptor_data& d);

void	perform_act(const char *orig, char_data *ch,
                    struct obj_data *obj, const void *vict_obj, char_data *to);

void	act(const std::string & str, int hide_invisible, char_data *ch,
            obj_data *obj, void *vict_obj, int type);

#define TO_ROOM		1
#define TO_VICT		2
#define TO_NOTVICT	3
#define TO_CHAR		4
#define TO_SLEEP	128	/* to char, even if sleeping */

/* I/O functions */
void	write_to_q(std::string_view txt, mods::descriptor_data&, int aliased);
int	write_to_descriptor(socket_t desc, const char *txt);
size_t write_to_output(mods::descriptor_data &t, const char *txt, ...) __attribute__((format(printf, 2, 3)));
size_t	vwrite_to_output(mods::descriptor_data &d, const char *format, va_list args);
size_t write_to_output(const mods::descriptor_data &t, const char *txt, ...) __attribute__((format(printf, 2, 3)));
size_t	vwrite_to_output(const mods::descriptor_data &d, const char *format, va_list args);
void	string_add(mods::descriptor_data d, char *str);
void	string_write(mods::descriptor_data d, char **txt, size_t len, long mailto, void *data);

#define PAGE_LENGTH	22
#define PAGE_WIDTH	80
void	page_string(mods::descriptor_data d, char *str, int keep_internal);

typedef RETSIGTYPE sigfunc(int);

