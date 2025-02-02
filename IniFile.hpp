#pragma once
#include <fstream>
#include <stack>
#include <string>

#include "IniFileParsed.hpp"

enum StateParse { KEY = 0, VALUE = 1, BLOCK = 2, COMMENT = 3, NEWLINESKIP = 4 };

class IniFileParser {
 private:
 public:
  IniFileParsed parse(const std::string string) const;
  IniFileParsed parse(std::fstream* stream) const;
  IniFileParser();
  ~IniFileParser();
};

IniFileParsed IniFileParser::parse(std::fstream* stream) const {
  if (stream->bad() || stream->fail()) {
    throw std::runtime_error("Something went wrong when opening configuration");
  }
  stream->seekg(0, std::ios::end);
  size_t maxSize = stream->tellg();
  stream->seekg(0, std::ios::beg);
  char* buffer = new char[maxSize];
  stream->read(buffer, maxSize);
  IniFileParsed parsed = parse(std::string(buffer));
  delete[] buffer;
  return parsed;
}

IniFileParsed IniFileParser::parse(const std::string string) const {
  std::stack<std::string> qualifierStack;
  IniFileParsed parse;
  std::string s = "";
  std::string block;
  int state = 0;
  for (size_t j = 0; j < string.length(); j++) {
    char i = string.at(j);
    if (i == ' ' || i == '\t') {
    } else if (state == KEY && s.empty() && (i == '#' || i == ';')) {
      state = COMMENT;
    } else if (state == KEY && s.empty() && i == '[') {
      state = BLOCK;
    } else if (state == BLOCK && !s.empty() && i == ']') {
      if (s.at(0) == '.') {
        qualifierStack.push(qualifierStack.top() + s);
      } else {
        qualifierStack.push(s);
      }
      s.clear();
    } else if (state == BLOCK && i == '\n') {
      state = KEY;
    } else if ((state == COMMENT || state == KEY) && i == '\n') {
      state = KEY;
      s.clear();
    } else if (state != NEWLINESKIP && i == '\n') {
      state = KEY;
      if (qualifierStack.size() == 0) {
        parse.add("", block, s);
      } else {
        parse.add(qualifierStack.top(), block, s);
      }
      s.clear();
    } else if (state == KEY && i == '=') {
      state = VALUE;
      block = s;
      s.clear();
    } else if (i == '\\') {
      state = NEWLINESKIP;
    } else {
      s += i;
    }
  }

  if (state == VALUE && !s.empty()) {
    if (qualifierStack.size() == 0) {
      parse.add("", block, s);
    } else {
      parse.add(qualifierStack.top(), block, s);
    }
  }
  return parse;
}

IniFileParser::IniFileParser() {}

IniFileParser::~IniFileParser() {}
