#include <iostream>

#include "IniFile.hpp"

int main() {
  IniFileParser parser;
  IniFileParsed parsed = parser.parse("hello=world");

  std::cout << parsed.get("hello") << std::endl;
  return 1;
}