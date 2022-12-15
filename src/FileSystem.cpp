#include "FileSystem.h"

#include <algorithm>
#include <iostream>
#include <stack>

struct FileSystem::File {
  std::string name;
  int32_t size;
};

struct FileSystem::Directory {
  int32_t size() const
  {
    if (size_ >= 0) {
      return size_;
    }
    size_ = 0;
    for (auto file : files) {
      size_ += file->size;
    }
    for (auto directory : directories) {
      size_ += directory->size();
    }
    return size_;
  }

  std::string name;
  std::shared_ptr<Directory> parent;
  std::vector<std::shared_ptr<Directory>> directories;
  std::vector<std::shared_ptr<File>> files;

 private:
  mutable int32_t size_ = -1;
};

FileSystem::FileSystem()
{
  root_ = std::make_shared<Directory>();
  current_ = root_;
}

FileSystem::~FileSystem() = default;

void FileSystem::parse(std::istream& in)
{
  std::string tmp, cmd, file;

  while (!in.eof()) {
    in >> tmp;
    if (tmp == "$") {
      // Command
      in >> cmd;
      if (cmd == "cd") {
        in >> file;
        if (file == "/") {
          current_ = root_;
        } else if (file == "..") {
          current_ = current_->parent;
        } else {
          for (const auto& dir : current_->directories) {
            if (dir->name == file) {
              current_ = dir;
            }
          }
        }
      }
      // ls is implied if its not a command
    } else {
      if (std::all_of(tmp.begin(), tmp.end(), isdigit)) {
        // File
        auto f = std::make_shared<File>();
        in >> f->name;
        f->size = std::stoi(tmp);
        current_->files.push_back(f);
      } else {
        // Directory
        auto d = std::make_shared<Directory>();
        in >> d->name;
        d->parent = current_;
        current_->directories.push_back(d);
      }
    }
  }
}

int32_t FileSystem::sumSmallestFiles()
{
  constexpr int32_t kCutoff = 100'000;
  std::stack<std::shared_ptr<Directory>> to_check;
  int32_t sum = 0;
  to_check.push(root_);

  while (!to_check.empty()) {
    auto dir = to_check.top();
    to_check.pop();

    if (dir->size() <= kCutoff) {
      sum += dir->size();
    }

    for (auto& subdir : dir->directories) {
      to_check.push(subdir);
    }
  }

  return sum;
}

int32_t FileSystem::findSmallestForDeletion()
{
  constexpr int32_t kTotal = 70'000'000;
  constexpr int32_t kRequired = 30'000'000;
  const int32_t available = kTotal - root_->size();
  const int32_t to_free = kRequired - available;

  int32_t smallest = std::numeric_limits<int32_t>::max();
  std::stack<std::shared_ptr<Directory>> to_check;
  to_check.push(root_);

  while (!to_check.empty()) {
    auto dir = to_check.top();
    to_check.pop();

    if (dir->size() >= to_free) {
      smallest = std::min(smallest, dir->size());

      // Only check children if this one is large enough
      for (auto& subdir : dir->directories) {
        to_check.push(subdir);
      }
    }

  }

  return smallest;
}
