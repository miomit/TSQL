#include <iostream>
#include "sql_cmd.hpp"

auto main() -> int {
    std::cout << "[CREATE - 0, INSERT - 1, DROP - 2, SELECT - 3, DELETE - 4]\n" << "> ";

    int c; std::cin >> c;
   
    switch (c) {
        case 0: return !sqlExe(sqlCreate);
        case 1: return !sqlExe(sqlInsert);
        case 2: return !sqlExe(sqlDrop);
        case 3: return !sqlExe(sqlSelect);
        case 4: return !sqlExe(sqlDelete);
    }

    return 1;
}
