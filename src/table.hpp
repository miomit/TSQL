#pragma once

#include <string>
#include <vector>
#include <memory>
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
    uint8_t size;

    TableHader() = default;
	TableHader(std::string str, Type t, uint8_t s): type{t}, size{s} {
		str.copy(name, sizeof(name));
	}
    static auto NUM(std::string name) -> TableHader {
    	return TableHader(name, INT, 4);
    }
    static auto SYMBOL(std::string name) -> TableHader {
    	return TableHader(name, CHAR, 1);
    }
 	static auto TEXT(std::string name, uint8_t size) -> TableHader {
    	return TableHader(name, CHAR, size);
    }
};

class table {
public:
	static auto open(std::string path) -> table;
	static auto create(std::string path, std::vector<TableHader> column) -> table;
    table(std::string path, std::vector<TableHader> column): 
    _path{path},
    _column{column} {
    	 _list = std::make_unique<flist>(path, 1 + sizeof(TableHader) * _column.size());
    };
    
private:
	std::unique_ptr<flist> _list;
    std::string _path;
    std::vector<TableHader> _column;
};