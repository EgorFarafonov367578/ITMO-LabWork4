//
// Created by User on 06.11.2022.
//

#pragma once

#include <fstream>
#include <iostream>
#include <queue>

class HamReader {
 private:
  std::fstream* file = NULL;
  uint64_t size_of_archive = 0;
  uint8_t rate = 0;
  uint8_t future_byte;
  uint8_t free_bite_in_future_byte;
  std::queue<uint8_t> counted_bytes;
  void PrepareBit(uint8_t bit);
 public:
  std::fstream*& GetFile() {

    return file;
  }
  void SetRate(uint8_t new_rate);
  uint8_t GetByte();
  void SetSizeOfArchive(uint64_t size) {
    size_of_archive = size;
  }
  void LoadFile(std::ofstream& file, uint32_t size);
};

uint64_t SizeOfFile(const std::string& file_name);