#include "flist.hpp"

auto Element::print() -> void {
    std::cout << "Adress: " << std::hex << adress << std::endl;;
    std::cout << "Data: ";
    for (int i = 0; i < data.size(); i++) std::cout << int(data[i]) << " ";
    std::cout << std::endl;
    std::cout << "Back: " << std::hex << back << std::endl;
    std::cout << "Next: " << std::hex << next << std::endl;
}

auto flist::size() -> uint16_t {
    return _size;
}

auto flist::append(std::vector<uint8_t> data) -> void {
    if (data.size() != _sizeData) throw "data.size() != _sizeData";

    for (uint16_t i = 0; i < 0x3FF; i++)
    {
        loadSection(i);
        auto row = getSectionFreeRow();
        if (row != -1) {
            setFlagUp(row);
            auto el = getElementByAdress(generateAddress(i, uint8_t(row)));
            el.data = data;
            el.back = _last;
            saveElement(el);

            if (_last != el.adress) {
                auto elBack = getElementByAdress(_last);
                elBack.next = el.adress;
                saveElement(elBack);
                setLast(el.adress);
            }

            setSize(_size + 1);

            return;
        }
    }

    throw "all cells are filled!!!";

}

auto flist::operator[] (const uint16_t index) -> std::vector<uint8_t> {
    return getElementByIndex(index).data;
}

auto flist::removeAt(const uint16_t index) -> void {
    Element el = getElementByIndex(index);
    setFlagDown(getSectionRowFromAdress(el.adress));
    setSize(_size - 1);

    if (_size == 0) {
        setFirst(0); setLast(0);
        return;
    }

    if (el.adress == _first) {
        setFirst(el.next);
    } else {
        Element elBack = getElementByAdress(el.back);
        elBack.next = el.next;
        saveElement(elBack);
    }

    if (el.adress == _last) {
        setLast(el.back);
    } else {
        Element elNext = getElementByAdress(el.next);
        elNext.back = el.back;
        saveElement(elNext);
    }
}

auto flist::getSectionIdFromAdress(uint16_t adress) -> uint16_t {
    return adress & uint16_t(0x3ff);
}

auto flist::getSectionRowFromAdress(uint16_t adress) -> uint8_t {
    return uint8_t(adress >> 10);
}

auto flist::getSectionFreeRow() -> int {
    if (~_isOccupied == uint16_t(0)) return -1;
    for (int i = 0; i < 64; i++) {
        if (!bool((_isOccupied >> i) & 1)){
            return i;
        }
    }
}

auto flist::getSeekByAdress(uint16_t adress) {
    return (_sizeData + 4) * getSectionRowFromAdress(adress);
}

auto flist::getElementByIndex(const uint16_t index) -> Element {
    if (index >= _size) { throw "segmentation fold"; }

    Element res = getElementByAdress(_first);

    for (int i = 0; i < index; i++) {
        res = getElementByAdress(res.next);
    }

    return res;
}

auto flist::getElementByAdress(uint16_t adress) -> Element {
    Element res;
    res.adress = adress;
    res.data = std::vector<uint8_t>(_sizeData, 0);

    loadSection(getSectionIdFromAdress(adress));

    auto seek = (_sizeData + 4) * getSectionRowFromAdress(adress);

    for (int i = 0; i < _sizeData; i++) res.data[i] = _sectionBuff[seek + i];

    res.back =  uint16_t(_sectionBuff[seek + _sizeData])
                | (uint16_t(_sectionBuff[seek + _sizeData + 1]) << 8);

    res.next =  uint16_t(_sectionBuff[seek + _sizeData + 2])
                | (uint16_t(_sectionBuff[seek + _sizeData + 3]) << 8);

    return res;
}

auto flist::setFlagUp (uint8_t id) -> void {
    _isOccupied |= uint64_t(1) << id;
    saveSection();
}

auto flist::setFlagDown (uint8_t id) -> void {
    _isOccupied &= ~(uint64_t(1) << id);
    saveSection();
}

auto flist::setSize(uint16_t val) -> void {
    _size = val;
    saveVal(_size, 2);
}

auto flist::setCountSection(uint8_t val) -> void {
    _countSection = val;
    saveVal(_countSection, 4);
}

auto flist::setFirst(uint16_t val) -> void {
    _first = val;
    saveVal(_first, 5);
}

auto flist::setLast(uint16_t val) -> void {
    _last = val;
    saveVal(_last, 7);
}

auto flist::saveElement(Element elem) -> void {
    if (elem.data.size() != _sizeData) throw "data.size() != _sizeData";
    loadSection(getSectionIdFromAdress(elem.adress));

    auto seek = getSeekByAdress(elem.adress);

    for (int i = 0; i < _sizeData; i++) _sectionBuff[seek + i] = elem.data[i];

    _sectionBuff[seek + _sizeData] = uint8_t(elem.back);
    _sectionBuff[seek + _sizeData + 1] = uint8_t(elem.back >> 8);

    _sectionBuff[seek + _sizeData + 2] = uint8_t(elem.next);
    _sectionBuff[seek + _sizeData + 3] = uint8_t(elem.next >> 8);

    saveSection();
}

template<typename T>
auto flist::saveVal(T val, size_t seek) -> void {
    std::fstream file(_pathFile, std::ios::binary | std::ios::out | std::ios::in);
    file.seekp(_beginRead + seek);
    file.write(reinterpret_cast<char*>(&val), sizeof(T));
    file.close();
}

auto flist::generateAddress(uint16_t sectionId, uint8_t sectionRow) -> uint16_t {
    return sectionId | (uint16_t(sectionRow) << 10);
}

auto flist::loadSection(uint16_t id) -> void {
    if (id > _countSection) {
        _isOccupied = 0;
        for (int i = 0; i < _sectionBuff.size(); i++) _sectionBuff[i] = 0;
        do
        {
            setCountSection(_countSection + 1);
            _sectionId = _countSection;
            saveSection();
        } while (_countSection != id);
        return;
    }
    _sectionId = id;
    std::fstream file(_pathFile, std::ios::binary | std::ios::in);
    file.seekg(_beginRead + _headerSize + id * (8 + _sectionBuff.size()), std::ios::beg);
    file.read(reinterpret_cast<char*>(&_isOccupied), sizeof(uint64_t));
    file.read(reinterpret_cast<char*>(_sectionBuff.data()), _sectionBuff.size());
    file.close();
}

auto flist::saveSection() -> void {
    std::fstream file(_pathFile, std::ios::binary | std::ios::out | std::ios::in);
    file.seekp(_beginRead + _headerSize + _sectionId * (8 + _sectionBuff.size()));
    file.write(reinterpret_cast<char*>(&_isOccupied), sizeof(uint64_t));
    file.write(reinterpret_cast<char*>(_sectionBuff.data()), _sectionBuff.size());
    file.close();
}