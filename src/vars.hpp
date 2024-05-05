#include <string>

const std::string dirPath = "/home/miomit/";

const std::string sqlCreate = "CREATE TABLE Students (Firstname CHAR(10), Surname CHAR(15), Age INT, Phone CHAR(9), SEX CHAR )";
const std::string sqlInsert = "INSERT INTO Students ( 'Yura', 'Movsesyan', 21, '+7(926) 33 80 800', 'M')";
const std::string sqlDrop = "DROP TABLE Students";
const std::string sqlSelect = "SELECT (Firstname, Age) FROM Students WHERE SEX = 'F' AND Age < 30 OR Surname = 'Movsesyan'";
const std::string sqlDelete = "DELETE FROM Students WHERE SEX = 'F'";
const std::string sqlUpdate = "UPDATE Students SET (Age = Age + 1) WHERE SEX = 'F'";