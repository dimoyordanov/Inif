#include <gtest/gtest.h>

#include <string>

#include "IniFile.hpp"
#include "IniFileParsed.hpp"

TEST(IniFileParse, TestSimpleSingleBlockParseOk) {
  IniFileParser parser;
  IniFileParsed parsed = parser.parse("[simple]\ntest=true");

  EXPECT_EQ(parsed.get("simple", "test"), "true");
}

TEST(IniFileParse, TestComplexSingleBlockParseOk) {
  IniFileParser parser;
  IniFileParsed parsed =
      parser.parse("[simple]\ntest=true\nmegatest=false\nmicrotest=true");

  EXPECT_EQ(parsed.get("simple", "megatest"), "false");
}

TEST(IniFileParse, TestSimpleMultiBlockParseOk) {
  IniFileParser parser;
  IniFileParsed parsed =
      parser.parse("[simple]\ntest=true\n[tensor]\nmath=yes\ngames=no");

  EXPECT_EQ(parsed.get("simple", "test"), "true");
  EXPECT_EQ(parsed.get("tensor", "games"), "no");
  EXPECT_EQ(parsed.get("tensor", "math"), "yes");
}

TEST(IniFileParse, TestSimpleMultiBlockParseTrimOk) {
  IniFileParser parser;
  IniFileParsed parsed =
      parser.parse("\n[simple]\ntest=true\n[tensor]\nmath=yes\ngames=no\n");

  EXPECT_EQ(parsed.get("simple", "test"), "true");
  EXPECT_EQ(parsed.get("tensor", "games"), "no");
  EXPECT_EQ(parsed.get("tensor", "math"), "yes");
}

TEST(IniFileParse, TestSimpleMultiBlockParseTrimSpacesOk) {
  IniFileParser parser;
  IniFileParsed parsed = parser.parse(
      "\n[simple]\ntest   =true\n[tensor]\nmath=   yes\ngames=no\n");

  EXPECT_EQ(parsed.get("simple", "test"), "true");
  EXPECT_EQ(parsed.get("tensor", "games"), "no");
  EXPECT_EQ(parsed.get("tensor", "math"), "yes");
}

TEST(IniFileParse, TestSimpleMultiBlockParseTabsOk) {
  IniFileParser parser;
  IniFileParsed parsed = parser.parse(
      "\n[simple]\n\ttest=true\n[tensor]\n\tmath=yes\n\tgames=no\n");

  EXPECT_EQ(parsed.get("simple", "test"), "true");
  EXPECT_EQ(parsed.get("tensor", "games"), "no");
  EXPECT_EQ(parsed.get("tensor", "math"), "yes");
}

TEST(IniFileParse, TestComplexMultiBlockParseOk) {
  IniFileParser parser;
  IniFileParsed parsed = parser.parse(
      "[simple]\ntest=true\nmegatest=false\nmicrotest=true\n[.gora]\ngorski=3");

  EXPECT_EQ(parsed.get("simple.gora", "gorski"), "3");
}

TEST(IniFileParse, TestBlockCommentDotCommaOk) {
  IniFileParser parser;
  IniFileParsed parsed =
      parser.parse("[simple]\ntest=true\n; megatest=false\nmicrotest=true");

  EXPECT_EQ(parsed.get("simple", "test"), "true");
  EXPECT_ANY_THROW(parsed.get("simple", "megatest"));
  EXPECT_EQ(parsed.get("simple", "microtest"), "true");
}

TEST(IniFileParse, TestBlockCommentDiesOk) {
  IniFileParser parser;
  IniFileParsed parsed =
      parser.parse("[simple]\ntest=true\n# megatest=false\nmicrotest=true");

  EXPECT_EQ(parsed.get("simple", "test"), "true");
  EXPECT_ANY_THROW(parsed.get("simple", "megatest"));
  EXPECT_EQ(parsed.get("simple", "microtest"), "true");
}

TEST(IniFileParse, TestSimpleParseFalse) {
  IniFileParser parser;
  IniFileParsed parsed = parser.parse("[simple]\ntest=true");

  EXPECT_ANY_THROW(parsed.get("simple", "nottest"));
}

TEST(IniFileParse, TestSimpleParseEmpty) {
  IniFileParser parser;
  IniFileParsed parsed = parser.parse("test=true");

  EXPECT_EQ(parsed.get("", "test"), "true");
}

TEST(IniFileParse, TestSimpleParseEmptyFunc) {
  IniFileParser parser;
  IniFileParsed parsed = parser.parse("test=true");

  EXPECT_EQ(parsed.get("test"), "true");
}

TEST(IniFileParse, TestSimpleParseEmptyFalse) {
  IniFileParser parser;
  IniFileParsed parsed = parser.parse("");

  EXPECT_ANY_THROW(parsed.get("simple", "nottest"));
}

TEST(IniFileParse, TestSimpleParseNewLineEmptyFalse) {
  IniFileParser parser;
  IniFileParsed parsed = parser.parse("\n\n");

  EXPECT_ANY_THROW(parsed.get("simple", "nottest"));
}

// Main function for Google Test
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}