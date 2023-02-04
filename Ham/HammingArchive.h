//
// Created by User on 12.11.2022.
//

#pragma once

#include <string>
#include <vector>

void AddFilesToArchive(const std::vector<std::string>& file_names, uint8_t rate, const std::string& archive_name);
void AddArchiveToArchive(const std::string& main_archive, const std::string& additional_archive);

void Delete(const std::vector<std::string>& file_names, const std::string& archive_name);
void Download(const std::vector<std::string>& file_names,const std::string& archive_name);
std::vector<std::string> ListOfFiles(const std::string& archive_name);