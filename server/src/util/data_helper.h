//
// Created by user on 15.07.2024.
//

#ifndef DATA_HELPER_H
#define DATA_HELPER_H

#include <vector>
#include <string>
#include <fstream>

#include "defines.h"

inline std::vector<u8> packet_from_file(const std::string& path) {
    // Read file as format: 0x01 0x02 0x03 0x04 ...
    std::ifstream file(path, std::ios::binary);
    std::vector<u8> data;

    if (file.is_open()) {
        while (!file.eof()) {
            u8 byte;
            file >> byte;
            data.push_back(byte);
        }
    }

    return data;
}

#endif //DATA_HELPER_H
