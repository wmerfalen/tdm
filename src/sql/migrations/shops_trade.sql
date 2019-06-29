CREATE TABLE shops_trade (
	shop_trade_id SERIAL,
	shop_fk_id INTEGER NOT NULL,
	shop_trade_type INTEGER NOT NULL,
	shop_trade_keywords VARCHAR(256) NOT NULL
);
