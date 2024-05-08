CREATE TABLE User (Name CHAR(20), Age INT, Sex CHAR, Count INT);

INSERT INTO User ('Yura', 21, 'M', 800000);
INSERT INTO User ('Tina', 34, 'F', 500);
INSERT INTO User ('David', 45, 'M', 200);
INSERT INTO User ('Janet', 17, 'F', 120);
INSERT INTO User ('Marie', 50, 'F', 120);
INSERT INTO User ('John', 90, 'M', 1000);
INSERT INTO User ('Bob', 12, 'M', 5000);
INSERT INTO User ('Anna', 23, 'F', 120);
INSERT INTO User ('Dora', 21, 'F', 120);

SELECT * FROM User;

SELECT (Name, Age) FROM User WHERE Age <= 28 AND Age > 17 AND Sex = 'F';

UPDATE User SET (Count = Count - 1000) WHERE Age > 17 OR Count >= 1000;

SELECT (Name, Count) FROM User;

DELETE FROM User WHERE Sex = 'F';

SELECT * FROM User;

DROP TABLE User;