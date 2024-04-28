#include "table.hpp"

auto main() -> int {
    
    /*table::create(
        "/home/miomit/my.tsql",
        std::vector<TableHader>{
            TableHader::NUM("id"),
            TableHader::TEXT("login", 50),
            TableHader::TEXT("password", 50),
        }
    );*/


    auto users = table::open("/home/miomit/my.tsql");

    users.insert(
        std::map<std::string, std::string> {
            {"id", "12321"},
            {"login", "yura"},
            {"password", "0123456789"}
        }
    );
}
