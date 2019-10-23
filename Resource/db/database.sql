CREATE TABLE IF NOT EXISTS `Register` (
	`idx`	INTEGER NOT NULL UNIQUE,  -- Face feature index
	`name`	TEXT,                     -- Name
	`no`	INTEGER NOT NULL UNIQUE,  -- Work number
	PRIMARY KEY(`idx`,`no`)
);
