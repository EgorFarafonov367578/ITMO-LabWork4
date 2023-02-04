//
// Created by User on 04.11.2022.
//
#include "Ham/HammingArchive.h"

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

bool is_number(const std::string& s) {
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it)) ++it;

  return !s.empty() && it == s.end();
}

int main(int argc, char* argv[]) {
  std::string archive;
  std::vector<std::string> file_names;
  std::cout << argv[1];
  if ((strcmp(argv[1] , "-c") == 0) || strcmp(argv[1], "--create") == 0 || strcmp(argv[1],"-a") == 0 || strcmp(argv[1],"--add") == 0) {
    int i = 2;
    std::cout << "Hi";
    while (i < argc) {
      if (argv[i][0] == '-') {
        if (strcmp(argv[i],"-f") == 0) {
          archive = argv[i+1];
          i = i + 2;
        } else {
          int j = 7;
          while (argv[i][j] != '\0') {
            archive.push_back(argv[i][j]);
          }

          i++;
        }
      } else {
        if (is_number(argv[i])) {
          AddFilesToArchive(file_names,std::stoi(argv[i]),archive);
        } else {
          file_names.push_back(argv[i]);
        }

        i++;
      }
    }
  } else if (strcmp(argv[1], "-l") == 0 || strcmp (argv[1], "--list") == 0) {
    std::vector<std::string> ans;
    if (strcmp(argv[2], "-f") == 0) {
      archive = argv[3];
    } else {
      int j = 7;
      while (argv[2][j] != '\0') {
        archive.push_back(argv[2][j]);
      }
    }

    ans = ListOfFiles(archive);
    for (int i = 0; i < ans.size(); i++) {
      std::cout << ans[i] << std::endl;
    }
  } else if (strcmp(argv[1],"-x")  == 0|| strcmp(argv[1],"--extract") == 0) {
    std::vector<std::string> ans;
    if (strcmp(argv[2], "-f") == 0) {
      archive = argv[3];
    } else {
      int j = 7;
      while (argv[2][j] != '\0') {
        archive.push_back(argv[2][j]);
      }
    }

    Download(ListOfFiles(archive),archive);
  } else if (strcmp(argv[1], "-d") == 0|| strcmp(argv[1], "--delete") == 0) {
    int i = 2;
    while (i < argc) {
      if (argv[i][0] == '-') {
        if (strcmp(argv[i], "-f") == 0) {
          archive = argv[i+1];
          i = i + 2;
        } else {
          int j = 7;
          while (argv[i][j] != '\0') {
            archive.push_back(argv[i][j]);
          }

          i++;
        }
      } else {
        file_names.push_back(argv[i]);
        i++;
      }
    }

    Delete(file_names,archive);
  } else if ((strcmp(argv[1], "-A") == 0) || (strcmp(argv[1],"--concatenate")) == 0) {
    int i = 2;
    while (i < argc) {
      if (argv[i][0] == '-') {
        if (strcmp(argv[i], "-f") == 0) {
          archive = argv[i+1];
          i = i + 2;
        } else {
          int j = 7;
          while (argv[i][j] != '\0') {
            archive.push_back(argv[i][j]);
          }

          i++;
        }
      } else {
        file_names.push_back(argv[i]);
        i++;
      }
    }

    for (int j = 0; j < file_names.size(); j++) {
      AddArchiveToArchive(archive,file_names[j]);
    }
  }
  return 0;
}
