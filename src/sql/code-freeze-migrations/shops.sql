CREATE TABLE shops (
	shop_id SERIAL,
	shop_vnum INTEGER NOT NULL,
	shop_title VARCHAR(256) NOT NULL,
	shop_description VARCHAR(2048) NOT NULL,
	shop_profit_buy FLOAT NOT NULL,
	shop_profit_sell FLOAT NOT NULL,       /* Factor to multiply cost with     */
	shop_type INTEGER NOT NULL,  /* Which items to trade         */
	shop_no_such_item1 VARCHAR(256) NOT NULL,     /* Message if keeper hasn't got an item */
	shop_no_such_item2 VARCHAR(256) NOT NULL,     /* Message if player hasn't got an item */
	shop_missing_cash1 VARCHAR(256) NOT NULL,     /* Message if keeper hasn't got cash    */
	shop_missing_cash2 VARCHAR(256) NOT NULL,     /* Message if player hasn't got cash    */
	shop_do_not_buy VARCHAR(256) NOT NULL,        /* If keeper dosn't buy such things */
	shop_message_buy VARCHAR(256) NOT NULL,       /* Message when player buys item    */
	shop_message_sell VARCHAR(256) NOT NULL,      /* Message when player sells item   */
	shop_temper1 INTEGER NOT NULL,       /* How does keeper react if no money    */
	shop_bitvector INTEGER NOT NULL, /* Can attack? Use bank? Cast here? */
	shop_keeper INTEGER NOT NULL,    /* The mobile who owns the shop (rnum)  */
	shop_with_who INTEGER NOT NULL,      /* Who does the shop trade with?    */
	shop_open1 INTEGER NOT NULL,
	shop_open2 INTEGER NOT NULL,      /* When does the shop open?     */
	shop_close1 INTEGER NOT NULL,
	shop_close2 INTEGER NOT NULL,    /* When does the shop close?        */
	shop_bankAccount INTEGER NOT NULL,       /* Store all gold over 15000 (disabled) */
	shop_lastsort INTEGER NOT NULL,      /* How many items are sorted in inven?  */
	shop_flags INTEGER
);

CREATE TABLE shop_rooms (
	shop_rooms_id SERIAL,
	shop_vnum INTEGER  NOT NULL,
	shop_room_vnum INTEGER NOT NULL
);

CREATE TABLE shop_objects (
	shop_objects_id SERIAL,
	shop_vnum INTEGER  NOT NULL,
	shop_object_vnum INTEGER NOT NULL
);

