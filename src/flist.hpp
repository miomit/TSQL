#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>

/*
|------------------------------------------|
|_sizeData|_size|_countSection|_first|_last|
|-----------+------------------------------|
|_isOccupied|_sectionBuff                  |
|-----------|------------------------------|
|...........|..............................|
|-----------|------------------------------|
|_isOccupied|_sectionBuff                  |
|-----------|------------------------------|
*/

struct Element {
    std::vector<uint8_t> data;
    uint16_t adress, back, next;

    auto print() -> void;
};

class flist {
public:
   flist(std::string pathFile, size_t beginRead = 0): _pathFile{pathFile}, _beginRead{beginRead} {
        std::ifstream file(_pathFile, std::ios::binary);

        if (!file.is_open()) {
            throw "flist: file is not exist!";
        }

        file.seekg(_beginRead);

        file.read(reinterpret_cast<char*>(&_sizeData), sizeof(uint16_t));
        file.read(reinterpret_cast<char*>(&_size), sizeof(uint16_t));
        file.read(reinterpret_cast<char*>(&_countSection), sizeof(uint8_t));
        file.read(reinterpret_cast<char*>(&_first), sizeof(uint16_t));
        file.read(reinterpret_cast<char*>(&_last), sizeof(uint16_t));

        _sectionBuff = std::vector<uint8_t>((_sizeData + 4) * 64, 0);

        loadSection(0);

        file.close();
    };

	auto getSizeData() -> size_t { return _sizeData; }

    auto size() -> uint16_t;
    auto append(std::vector<uint8_t> data) -> void;
    auto operator[] (const uint16_t index) -> std::vector<uint8_t>;
    auto removeAt(const uint16_t index) -> void;
	auto updateAt(const uint16_t index, std::vector<uint8_t> data) -> void;
private:
    std::string _pathFile;
    uint16_t _first{}, _last{};
    uint16_t _size, _sizeData;
    uint8_t _countSection{0};
    uint64_t _isOccupied{0};
    uint16_t _sectionId{0};
    std::vector<uint8_t> _sectionBuff;
    size_t _beginRead;
    size_t _headerSize = sizeof(_size) + sizeof(_sizeData) + sizeof(_countSection) + sizeof(_first) + sizeof(_last);

    auto getSectionIdFromAdress(uint16_t adress) -> uint16_t;
    auto getSectionRowFromAdress(uint16_t adress) -> uint8_t;
    auto getSectionFreeRow() -> int;
    auto getSeekByAdress(uint16_t adress);
    auto getElementByIndex(const uint16_t index) -> Element;
    auto getElementByAdress(uint16_t adress) -> Element;

    auto setFlagUp (uint8_t id) -> void;
    auto setFlagDown (uint8_t id) -> void;
    auto setSize(uint16_t val) -> void;
    auto setCountSection(uint8_t val) -> void;
    auto setFirst(uint16_t val) -> void;
    auto setLast(uint16_t val) -> void;

    auto saveElement(Element elem) -> void;

    template<typename T>
    auto saveVal(T val, size_t seek) -> void;

    auto generateAddress(uint16_t sectionId, uint8_t sectionRow) -> uint16_t;

    auto loadSection(uint16_t id) -> void;
    auto saveSection() -> void;
};