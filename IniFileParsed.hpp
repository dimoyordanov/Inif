#pragma once

#include <string>
#include <unordered_map>

class IniFileParsed {
 private:
 public:
  std::unordered_map<std::string, std::unordered_map<std::string, std::string> >
      values;
  void add(std::string block, std::string name, std::string value);
  std::string get(std::string block, std::string name);
  std::string get(std::string name);
  IniFileParsed();
  ~IniFileParsed();
};

void IniFileParsed::add(std::string block, std::string name,
                        std::string value) {
  if (this->values.count(block) == 0) {
    std::unordered_map<std::string, std::string> map;
    this->values.insert(
        std::pair<std::string, std::unordered_map<std::string, std::string> >(
            block, map));
  }
  this->values.at(block).insert(
      std::pair<std::string, std::string>(name, value));
}

std::string IniFileParsed::get(std::string name) { return this->get("", name); }

std::string IniFileParsed::get(std::string block, std::string name) {
  if (this->values.count(block) == 0 ||
      this->values.at(block).count(name) == 0) {
    throw std::runtime_error("No config with this name.");
  }
  return this->values.at(block).at(name);
}

IniFileParsed::IniFileParsed() {}

IniFileParsed::~IniFileParsed() {}
