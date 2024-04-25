#pragma once

#include <string>
#include <vector>
#include "flist.hpp"

/*
|----------------------------------|
|_countClumns|_column[_countClumns]|
|------------+---------------------|
|_list                             |
|----------------------------------|
*/

enum Type { INT, CHAR };

struct TableHader
{
    char name[50];
    Type type;
    unsigned int size;
};

class table {
public:
    table(std::string path, std::vector<TableHader> column): 
    _path{path},
    _column{column},
    _list{_path, 2 + sizeof(TableHader) * _column.size()} { };
    
private:
    flist _list;
    std::string _path;
    std::vector<TableHader> _column;
};
