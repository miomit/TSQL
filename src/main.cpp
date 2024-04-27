#include "table.hpp"

auto main() -> int {
    
    table::create(
        "/home/miomit/my.tsql",
        std::vector<TableHader>{
            TableHader::NUM("id"),
            TableHader::TEXT("login", 50),
            TableHader::TEXT("password", 50),
        }
    );


    table::open("/home/miomit/my.tsql");
}
