CREATE TABLE `FaceFeature` (
	`index`	INTEGER NOT NULL UNIQUE,
	`name`	TEXT,
	`no`	INTEGER NOT NULL UNIQUE,
	PRIMARY KEY(`index`,`no`)
);
