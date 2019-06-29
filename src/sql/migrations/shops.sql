CREATE TABLE shops (
	shop_id SERIAL,
	shop_vnum INTEGER NOT NULL,
	shop_profit_buy INTEGER NOT NULL,
	shop_profit_sell INTEGER NOT NULL,       /* Factor to multiply cost with     */
	shop_type INTEGER NOT NULL,  /* Which items to trade         */
	shop_no_such_item1 INTEGER NOT NULL,     /* Message if keeper hasn't got an item */
	shop_no_such_item2 INTEGER NOT NULL,     /* Message if player hasn't got an item */
	shop_missing_cash1 INTEGER NOT NULL,     /* Message if keeper hasn't got cash    */
	shop_missing_cash2 INTEGER NOT NULL,     /* Message if player hasn't got cash    */
	shop_do_not_buy INTEGER NOT NULL,        /* If keeper dosn't buy such things */
	shop_message_buy INTEGER NOT NULL,       /* Message when player buys item    */
	shop_message_sell INTEGER NOT NULL,      /* Message when player sells item   */
	shop_temper1 INTEGER NOT NULL,       /* How does keeper react if no money    */
	shop_bitvector INTEGER NOT NULL, /* Can attack? Use bank? Cast here? */
	shop_keeper INTEGER NOT NULL,    /* The mobile who owns the shop (rnum)  */
	shop_with_who INTEGER NOT NULL,      /* Who does the shop trade with?    */
	shop_in_room INTEGER NOT NULL,      /* Where is the shop?           */
	shop_open1 INTEGER NOT NULL,
	shop_open2 INTEGER NOT NULL,      /* When does the shop open?     */
	shop_close1 INTEGER NOT NULL,
	shop_close2 INTEGER NOT NULL,    /* When does the shop close?        */
	shop_bankAccount INTEGER NOT NULL,       /* Store all gold over 15000 (disabled) */
	shop_lastsort INTEGER NOT NULL      /* How many items are sorted in inven?  */
);

CREATE TABLE shops_producing (
	shop_producing_id SERIAL,
	shop_fk_id INTEGER NOT NULL,
   	shop_producing_obj_vnum INTEGER NOT NULL
);

CREATE TABLE shops_trade (
	shop_trade_id SERIAL,
	shop_fk_id INTEGER NOT NULL,
	shop_trade_type INTEGER NOT NULL,
	shop_trade_keywords VARCHAR(256) NOT NULL
);
