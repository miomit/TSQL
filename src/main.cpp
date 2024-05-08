#include <iostream>
#include <string>
#include "sql_cmd.hpp"

auto main(int argc, char* argv[]) -> int {

    if (argc > 1) {
        std::ifstream file(argv[1]);

        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл." << std::endl;
            return 1;
        }

        char c;
        std::string buff = "";

        while (file.get(c)) {
            if (c != ';') {
                buff += c;
            } else {
                try {
                    sqlExe(buff);
                } catch (...) {
                    std::cout << "Error in " << buff << std::endl;
                    return 1;
                }
                buff = "";
            }
        }

        file.close();
    }

    return 1;
}
