//
// Created by User on 04.11.2022.
//

#include "HamWriter.h"

uint64_t SizeOfFile(const std::string& file_name) {
  std::ifstream file(file_name, std::ios::binary| std::ios::ate|std::ios::in);
  uint64_t size = file.tellg();
  file.close();

  return size;
}

void HamWriter::PutBit(uint8_t bite) {
  future_row[free_bite_in_future_row / 8] += bite * (1 << (free_bite_in_future_row % 8));
  free_bite_in_future_row++;
  if (free_bite_in_future_row == last_bit) {
    uint32_t control_number = 0;
    for (int i = 1; i < last_bit; i++) {
      control_number = control_number ^ (((future_row[i / 8] & (1 << (i % 8))) != 0) ? i : 0);
    }

    for (int i = 0; i < 32; i++) {
      if ((control_number & (1 << i)) != 0) {
        future_row[(1 << i) / 8] += (1 << ((1 << i) % 8));
      }
    }

    bool sum = false;
    for (int i = 0; i < (1 << rate); i++) {
      sum = sum ^ ((future_row[i / 8] & (1 << (i % 8))) != 0);
    }

    future_row[0] += (sum ? 1 : 0);
    for (int i = 0; i < (1 << (rate - 3)); i++) {
      (*file) << uint8_t (future_row[i]);
    }

    for (int i = 0; i < (1 << (rate - 3)); i++) {
      future_row[i] = 0;
    }

    future_control_bit = 4;
    free_bite_in_future_row = 3;
    last_bit = (1 << rate);
  } else if (free_bite_in_future_row == future_control_bit) {
    free_bite_in_future_row++;
    future_control_bit *= 2;
  }
}

void HamWriter::SetRate(uint8_t new_rate) {
  if (free_bite_in_future_row != 3) {
    while (free_bite_in_future_row != 3) {
      PutBit(0);
    }
  }

  rate = new_rate;
  last_bit = (1<<rate);
  future_row = new uint8_t [(1<<(rate-3))];
  for (int i = 0; i < (1 << (rate - 3)); i++) {
    future_row[i] = 0;
  }

  free_bite_in_future_row = 3;
  future_control_bit = 4;
}
