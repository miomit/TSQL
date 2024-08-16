build

```bash
sudo chmod +x ./build

./build
```

exemple

```sql
CREATE TABLE User (Name CHAR(20), Age INT, Sex CHAR, Count INT);

INSERT INTO User ('Yura', 21, 'M', 1000);

SELECT * FROM User;
SELECT * FROM User WHERE Count > 100 AND Count < 1000;
SELECT (Name, Count) FROM User;

UPDATE User SET (Sex = 'F') WHERE Count = 5000;
UPDATE User SET (Age = Age + 10) WHERE Count > 100 AND Count < 1000;
UPDATE User SET (Age = Age - 10, Name = 'Bob', Count = 0);
WHERE Count > 100 AND Count < 1000;

DELETE FROM User WHERE Count <= 500;
DELETE FROM User WHERE Count = 0 AND Sex = 'F';

DROP TABLE User;
```