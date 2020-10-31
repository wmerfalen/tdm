
extern void send_to_room_except(room_rnum room, player_ptr_t except_me, const char *messg, ...);
extern void send_to_room_except(room_rnum, const std::vector<char_data*>&, const char *, ...);
extern void send_to_room_except(room_rnum room, player_ptr_t except_me, const char *messg, ...);
extern void send_to_room_except(room_rnum room, const std::vector<player_ptr_t>& except_me, const char *messg, ...);
extern void send_to_room_except(room_rnum room, std::vector<uuid_t> except_me, const char *messg, ...);
