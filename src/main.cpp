#include "flist.hpp"

auto main() -> int {
    flist list("/home/miomit/Документы/flist.list", 0);

    list.append(std::vector<uint8_t>{1});
    for (int i = 0; i < list.size(); i++)
        std::cout << int(list[i][0]) << std::endl;
}
