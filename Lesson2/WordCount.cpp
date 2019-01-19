// Copyright 2019 Michael Johnson

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>

#include "../common.h"

namespace mjohnson {
namespace wordcount {
// Runs unit tests to confirm program functionality
bool RunUnitTests();
int CountWords(const std::string);
int CountWords(const char*);
bool IsLetter(const char);

void Run() {
  mjohnson::common::ClearScreen();

  if (RunUnitTests()) {
    std::cout << "Unit tests passed." << std::endl;
  } else {
    std::cout << "Unit tests failed." << std::endl;
    return;
  }

  do {
    std::cout << "" << std::endl;
    const std::string input = mjohnson::common::RequestInput<std::string>(
        "Enter a sentence or phrase to have its words counted:\n", NULL);
    const int words = CountWords(input);
    std::cout << "\"" << input << "\" has " << words << " words in it."
              << std::endl;
  } while (mjohnson::common::RequestContinue());
}

int CountWords(const std::string str) { return CountWords(str.c_str()); }

int CountWords(const char* str) {
  if (str[0] == '\0') {
    // This is a zero-length string; shortcut
    return 0;
  }

  // Initialize based on first character
  int words = (IsLetter(str[0]) ? 1 : 0);
  bool lastWasWord = IsLetter(str[0]);

  for (size_t i = 1; str[i] != '\0';
       i++) {  // Skip the first character; we already processed it
    const char c = str[i];
    if (!IsLetter(c)) {
      lastWasWord = false;
      continue;
      // This is a non-word; don't count words
    }

    if (lastWasWord) {
      continue;
      // This is a word following a word; don't count a new word
    }

    // This is a word following a non-word; count a word
    words += 1;
    lastWasWord = true;  // Don't count the next characters as words
  }

  return words;
}

bool IsLetter(const char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// TestCase represents a unit test case. It contains a string to be tested and
// the expected output of the CountWords function.
struct TestCase {
  // The string to be tested
  std::string example;
  // The expected output of CountWords
  int expected_words;
};

bool RunUnitTests() {
  static const TestCase CASES[] = {
      {"", 0},  // Spaces aren't words; expect 0
      {" ", 0},
      {"  ", 0},
      {"0", 0},  // Numbers aren't words; expect 0
      {" 0", 0},
      {"0 ", 0},
      {" 0 ", 0},
      {"_", 0},  // Special characters aren't words; expect 0
      {" _", 0},
      {"_ ", 0},
      {" _ ", 0},
      {"Test", 1},  // One word
      {"  Test", 1},
      {"Test  ", 1},
      {"  Test  ", 1},
      {"Test  case", 2},  // Two words
      {"  Test case", 2},
      {"Test  case  ", 2},
      {"  Test  case  ", 2},
      {"T e s t", 4},  // Four "words"
      {" T e s t", 4},
      {"T e s t ", 4},
      {" T e s t ", 4},
  };

  for (auto test_case : CASES) {
    const int words = CountWords(test_case.example);
    if (words != test_case.expected_words) {
      std::cout << "Unit test failed: Expected " << test_case.expected_words
                << " words, got " << words << std::endl;
      return false;
    }
  }

  return true;
}
}  // namespace wordcount
}  // namespace mjohnson

int main() { mjohnson::wordcount::Run(); }
