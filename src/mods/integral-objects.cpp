#include "integral-objects.hpp"
#include "help.hpp"
#include "super-users.hpp"

namespace mods::integral_objects {
	/*
CREATE TABLE integral_object (
	object_id SERIAL,
	object_room_vnum INTEGER NOT NULL,
	object_type VARCHAR(16) NOT NULL,
	object_vnum INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE camera_feed (
	feed_id SERIAL,
	feed_type VARCHAR(16) NOT NULL,
	feed_vnum INTEGER NOT NULL,
	feed_room_vnum INTEGER NOT NULL,
	feed_order INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
*/

};

ACMD(do_install_camera_feed){
	DO_HELP("install_camera_feed");
	ADMIN_REJECT();
	/** code here */
	ADMIN_DONE();
}

ACMD(do_uninstall_camera_feed){
	DO_HELP("uninstall_camera_feed");
	ADMIN_REJECT();
	/** code here */
	ADMIN_DONE();
}

ACMD(do_install_computer_choice){
	DO_HELP("install_computer_choice");
	ADMIN_REJECT();
	/** code here */
	ADMIN_DONE();
}
