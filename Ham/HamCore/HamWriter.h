//
// Created by User on 04.11.2022.
//
#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>

class HamWriter {
 private:
  std::fstream* file = NULL;
  uint8_t rate = 0;
  uint8_t* future_row = NULL;
  uint32_t free_bite_in_future_row = 3;
  uint32_t future_control_bit = 0;
  uint32_t last_bit = 0;
  void PutBit(uint8_t bite);
 public:
  std::fstream*& GetFile() {

    return file;
  }
  void SetRate(uint8_t new_rate);
  void WriteByte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
      PutBit((((byte & (1 << i)) != 0)? 1: 0));
    }
  }
};

uint64_t SizeOfFile(const std::string& file_name);