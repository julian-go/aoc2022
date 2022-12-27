#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

#include <memory>
#include <string>
#include <vector>

class FileSystem {
  struct File;
  struct Directory;

 public:
  FileSystem();
  virtual ~FileSystem();

  void parse(std::istream& in);

  int32_t sumSmallestFiles();         // day07-1
  int32_t findSmallestForDeletion();  // day07-2

 private:
  std::shared_ptr<Directory> root_;
  std::shared_ptr<Directory> current_;
};

#endif  // !FILE_SYSTEM_H_
