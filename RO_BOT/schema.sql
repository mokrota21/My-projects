DROP TABLE IF EXISTS "items";
DROP TABLE IF EXISTS "sellers";
DROP TABLE IF EXISTS "purchases";
CREATE TABLE "items"
(
    "id"         INTEGER PRIMARY KEY,
    "item_id"    INTEGER NOT NULL,
    "item_name"  TEXT,
    "store_id"   INTEGER,
    "store_name" TEXT,
    "amount"     INTEGER,
    "price"      INTEGER,
    "x"          INTEGER,
    "y"          INTEGER
);
CREATE TABLE "sellers"
(
    "id"          INTEGER PRIMARY KEY,
    "seller_name" TEXT
);
CREATE TABLE "purchases"
(
    "id"          INTEGER PRIMARY KEY,
    "seller_id"   INTEGER,
    "seller_name" TEXT,
    "item_id"     INTEGER,
    "amount"      INTEGER,
    "price"       INTEGER,
    "last_seen"   DATE
);