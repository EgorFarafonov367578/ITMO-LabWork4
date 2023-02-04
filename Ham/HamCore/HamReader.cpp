//
// Created by User on 06.11.2022.
//

#include "HamReader.h"

void HamReader::PrepareBit(uint8_t bit) {
  future_byte += bit * (1 << free_bite_in_future_byte);
  free_bite_in_future_byte++;
  if (free_bite_in_future_byte == 8) {
    counted_bytes.push(future_byte);
    future_byte = 0;
    free_bite_in_future_byte = 0;
  }
}

uint8_t HamReader:: GetByte() {
  if (!counted_bytes.empty()) {
    uint8_t byte = counted_bytes.front();
    counted_bytes.pop();
    return byte;
  }

  uint8_t* row = new uint8_t[(1<<(rate-3))];
  for (int i = 0; i < (1<<(rate-3)); i++) {
    row[i] = (*file).get();
  }

  bool new_sum = false;
  for (int i = 1; i < (1<<rate); i++) {
    new_sum = new_sum ^ ((row[i / 8] & (1 << (i % 8))) != 0);
  }

  uint32_t new_control_number = 0;
  for (int i = 1; i < (1<<rate); i++) {
    new_control_number = new_control_number ^ (((row[i / 8] & (1 << (i % 8))) != 0) ? i : 0);
  }

  bool old_sum = ((row[0] % 2) == 1? true : false);
  if ((new_control_number != 0) && (new_sum != old_sum)) {
    row[new_control_number/8] = row[new_control_number/8] ^ (1 << (new_control_number % 8));
    std::cout << "Error was fixed";
  }

  if ((new_control_number != 0) && (new_sum == old_sum)) {
    std::cout << "File is broken" << std::endl;
    for (int i = 0; i < (1<<rate); i++) {
      std::cout << (((row[i/8] & (1 << (i%8))) != 0)? 1:0);
    }
    std::cout <<std::endl <<  old_sum << new_sum << std::endl;
    exit(1);
  }

  int future_control_bit = 4;
  for (int i = 3; i < (1<<rate); i++) {
    if (i == future_control_bit) {
      future_control_bit *= 2;
    } else {
      PrepareBit((((row[i / 8] & (1 << (i % 8))) != 0)? 1: 0));
    }
  }

  delete[] row;
  return GetByte();
}

void HamReader::SetRate(uint8_t new_rate) {
  rate = new_rate;
  counted_bytes = std::queue<uint8_t>();
  future_byte = 0;
  free_bite_in_future_byte = 0;
}

void HamReader::LoadFile(std::ofstream& file, uint32_t size) {
  for (uint32_t i = 0; i < size; i++) {
    file << GetByte();
  }
}