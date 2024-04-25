#include <iostream>
#include <vector>

import flist;

auto main() -> int {
    flist list("/home/miomit/Документы/flist.list", 16);

    std::cout << int(list[20][0]) << std::endl;
}
