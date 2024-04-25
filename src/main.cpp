#include <iostream>
#include <vector>

import flist;

auto main() -> int {
    flist list("/home/miomit/Документы/flist.list", 16);

    list.removeAt(list.size() - 1);
    for (int i = 0; i < list.size(); i++)
        std::cout << int(list[i][0]) << std::endl;
}
