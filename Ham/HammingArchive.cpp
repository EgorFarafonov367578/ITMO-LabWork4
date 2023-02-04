//
// Created by User on 12.11.2022.
//

#include "HamCore/HamReader.h"
#include "HamCore/HamWriter.h"
#include "HammingArchive.h"

#include <filesystem>
#include <iostream>
#include <set>


void AddFileToArchive(const std::string& file_name, uint8_t rate, const std::string& archive_name) {
  HamWriter hw;
  std::fstream* arch_file = new std::fstream (archive_name,std::ios::binary|std::ios::out|std::ios::app);
  hw.GetFile() = arch_file;
  uint64_t size = SizeOfFile(file_name);
  uint64_t size2 = size;
  hw.SetRate(3);
  hw.WriteByte(rate);
  for (int i = 0; i < file_name.length(); i++) {
    hw.WriteByte(file_name[i]);
  }

  hw.WriteByte(0);
  hw.WriteByte(size % (1<<8));
  size /= (1<<8);
  hw.WriteByte(size % (1<<8));
  size /= (1<<8);
  hw.WriteByte(size % (1<<8));
  size /= (1<<8);
  hw.WriteByte(size % (1<<8));

  hw.SetRate(rate);
  std::ifstream file(file_name,std::ios::binary|std::ios::in);
  uint8_t byte;
  for (int i = 0; i < size2; i++) {
    byte = file.get();
    hw.WriteByte(byte);
  }

  hw.SetRate(3);
  file.close();
  hw.GetFile()->close();
  delete hw.GetFile();
}

void AddFilesToArchive(const std::vector<std::string>& file_names, uint8_t rate, const std::string& archive_name) {
  for (int i = 0; i < file_names.size();i++) {
    AddFileToArchive(file_names[i],rate,archive_name);
  }
}

void AddArchiveToArchive(const std::string& main_archive_name, const std::string& additional_archive_name) {
  uint64_t additional_archive_size = SizeOfFile(additional_archive_name);
  std::ofstream main_archive(main_archive_name,std::ios::binary|std::ios::out|std::ios::app);
  std::ifstream additional_archive(additional_archive_name,std::ios::binary|std::ios::out);
  uint8_t byte;
  for (int i = 0; i < additional_archive_size; i++) {
    byte = additional_archive.get();
    main_archive << byte;
  }

  main_archive.close();
  additional_archive.close();
}

void Delete(const std::vector<std::string>& file_names, const std::string& archive_name) {
  std::set<std::string> files_needed_to_delete;
  for (int i = 0; i < file_names.size(); i++) {
    files_needed_to_delete.insert(file_names[i]);
  }

  uint64_t size_of_archive = SizeOfFile(archive_name);
  HamReader hr;
  hr.GetFile() = new std::fstream (archive_name,std::ios::binary|std::ios::in| std::ios::out);
  hr.SetSizeOfArchive(size_of_archive);
  uint64_t start_of_archive = 0;
  while (true) {
    if ((*hr.GetFile()).tellg() != size_of_archive) {
      uint64_t start_of_file = (*hr.GetFile()).tellg();
      hr.SetRate(3);
      uint8_t file_rate = hr.GetByte();
      std::string current_file_name;
      uint8_t file_name_char = hr.GetByte();
      while (file_name_char != 0) {
        current_file_name.push_back(char(file_name_char));
        file_name_char = hr.GetByte();
      }

      uint32_t file_size = 0;
      uint64_t first_byte = hr.GetByte();
      uint64_t second_byte = hr.GetByte();
      uint64_t theird_byte = hr.GetByte();
      uint64_t fourth_byte = hr.GetByte();
      file_size = first_byte + second_byte * (1<<8) + theird_byte * (1 << 16) + fourth_byte * (1 << 24);
      hr.SetRate(3);
      uint64_t file_size_in_bits = file_size * 8;
      uint64_t bits_in_row = ((1<<file_rate) - (file_rate + 1));
      uint64_t number_of_rows = (file_size_in_bits + bits_in_row - 1) / bits_in_row;
      uint64_t ham_file_size = number_of_rows * (1<<(file_rate - 3));
      (*hr.GetFile()).seekg(long(ham_file_size), std::ios::cur);
      uint64_t end_of_file = (*hr.GetFile()).tellg();
      if (files_needed_to_delete.find(current_file_name) == files_needed_to_delete.end()) {
        for (int i = start_of_file; i < end_of_file; i++) {
          uint8_t byte;
          (*hr.GetFile()).seekg(i);
          byte = (*hr.GetFile()).get();
          (*hr.GetFile()).seekg(start_of_archive);
          (*hr.GetFile()) << byte;
          start_of_archive++;
        }

        (*hr.GetFile()).seekg(end_of_file);
      }
      else {
        continue;
      }
    }
    else {
      break;
    }
  }

  hr.GetFile()->close();
  delete hr.GetFile();
  std::filesystem::path p = archive_name;
  std::filesystem::resize_file(p, start_of_archive);
}

void Download(const std::vector<std::string>& file_names,const std::string& archive_name) {
  std::set<std::string> files_needed_to_be_loaded;
  for (int i = 0; i < file_names.size(); i++) {
    files_needed_to_be_loaded.insert(file_names[i]);
  }

  uint64_t size_of_archive = SizeOfFile(archive_name);
  HamReader hr;
  hr.GetFile() = new std::fstream (archive_name,std::ios::binary|std::ios::in| std::ios::out);
  hr.SetSizeOfArchive(size_of_archive);
  while (true) {
    if ((*hr.GetFile()).tellg() != size_of_archive) {
      hr.SetRate(3);
      uint8_t file_rate = hr.GetByte();
      std::string current_file_name;
      uint8_t file_name_char = hr.GetByte();
      while (file_name_char != 0) {
        current_file_name.push_back(char(file_name_char));
        file_name_char = hr.GetByte();
      }

      uint32_t file_size = 0;
      uint64_t first_byte = hr.GetByte();
      uint64_t second_byte = hr.GetByte();
      uint64_t theird_byte = hr.GetByte();
      uint64_t fourth_byte = hr.GetByte();
      file_size = first_byte + second_byte * (1<<8) + theird_byte * (1 << 16) + fourth_byte * (1 << 24);
      hr.SetRate(3);
      std::cout << current_file_name << std::endl;
      if (files_needed_to_be_loaded.find(current_file_name) == files_needed_to_be_loaded.end()) {
        uint64_t file_size_in_bits = file_size * 8;
        uint64_t bits_in_row = ((1<<file_rate) - (file_rate + 1));
        uint64_t number_of_rows = (file_size_in_bits + bits_in_row - 1) / bits_in_row;
        uint64_t ham_file_size = number_of_rows * (1<<(file_rate - 3));
        (*hr.GetFile()).seekg(long(ham_file_size), std::ios::cur);
      }
      else {
        std::ofstream out_file(current_file_name,std::ios::binary|std::ios::out);
        hr.SetRate(file_rate);
        hr.LoadFile(out_file,file_size);
        out_file.close();
        hr.SetRate(3);
      }
    }
    else {
      break;
    }
  }

  hr.GetFile()->close();
  delete hr.GetFile();
}

std::vector<std::string> ListOfFiles(const std::string& archive_name) {
  uint64_t size_of_archive = SizeOfFile(archive_name);
  HamReader hr;
  hr.GetFile() = new std::fstream (archive_name,std::ios::binary|std::ios::in| std::ios::out);
  hr.SetSizeOfArchive(size_of_archive);
  std::vector<std::string> ans;
  while (true) {
    if ((*hr.GetFile()).tellg() != size_of_archive) {
      hr.SetRate(3);
      uint8_t file_rate = hr.GetByte();
      uint8_t file_name_char = hr.GetByte();
      std::string file_name;
      while (file_name_char != 0) {
        file_name.push_back(char(file_name_char));
        file_name_char = hr.GetByte();
      }

      ans.push_back(file_name);
      uint32_t file_size = 0;
      uint64_t first_byte = hr.GetByte();
      uint64_t second_byte = hr.GetByte();
      uint64_t theird_byte = hr.GetByte();
      uint64_t fourth_byte = hr.GetByte();
      file_size = first_byte + second_byte * (1<<8) + theird_byte * (1 << 16) + fourth_byte * (1 << 24);
      hr.SetRate(3);
      uint64_t file_size_in_bits = file_size * 8;
      uint64_t bits_in_row = ((1<<file_rate) - (file_rate + 1));
      uint64_t number_of_rows = (file_size_in_bits + bits_in_row - 1) / bits_in_row;
      uint64_t ham_file_size = number_of_rows * (1<<(file_rate - 3));
      (*hr.GetFile()).seekg(long(ham_file_size), std::ios::cur);
      hr.SetRate(3);
    }
    else {
      break;
    }
  }

  hr.GetFile()->close();
  delete hr.GetFile();

  return ans;
}