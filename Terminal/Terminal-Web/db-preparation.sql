CREATE TABLE appointments(
	ID	INTEGER PRIMARY KEY AUTO_INCREMENT,
	date	VARCHAR(2) NOT NULL,
	time	VARCHAR(2) NOT NULL,
	taken	BOOLEAN NOT NULL,
	timestamp	INT(11)
);

INSERT INTO appointments(
  (date, time, taken)
) VALUES
  (8, 7, FALSE),
  (8, 8, FALSE)

UPDATE appointments
SET taken = TRUE, timestamp = 1526043652
WHERE date = 8 AND time = 7