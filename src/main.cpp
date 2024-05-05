#include <iostream>
#include <string>
#include "sql_cmd.hpp"

auto main(int argc, char* argv[]) -> int {

    if (argc > 1) {
        return !sqlExe(std::string(argv[1]));
    } else {
        std::cout << "[CREATE - 0, INSERT - 1, DROP - 2, SELECT - 3, DELETE - 4, UPDATE - 5]\n" << "> ";

        int c; std::cin >> c;
    
        switch (c) {
            case 0: return !sqlExe(sqlCreate);
            case 1: return !sqlExe(sqlInsert);
            case 2: return !sqlExe(sqlDrop);
            case 3: return !sqlExe(sqlSelect);
            case 4: return !sqlExe(sqlDelete);
            case 5: return !sqlExe(sqlUpdate);
        }
    }

    return 1;
}
