// Copyright 2019 Michael Johnson

#include <iostream>

#include "../common.h"

namespace mjohnson {
namespace digitsum {
bool CheckInput(const char);
bool CheckInput(const char[], size_t);
uint8_t CharToNum(const char);
uint64_t SumDigits(const char[], size_t);

void Run() {
  do {
    mjohnson::common::ClearScreen();
  } while (mjohnson::common::RequestContinue());
}

uint8_t CharToNum(const char c) {
  if (!CheckInput(c)) {
    throw new std::invalid_argument("c");
  }
  return (9 - ('9' - c));
}

bool CheckInput(const char c) { return (c >= '0' && c <= '9'); }

bool CheckInput(const char str[], size_t size) {
  for (size_t i = 0; str[i] != '\0' && i < size; i++) {
    const char c = str[i];
    if (!CheckInput(c)) {  // The given character isn't a number
      std::cout << "\"" << c << "\" is not a digit." << std::endl;
      return false;
    }
  }

  return true;
}

uint64_t SumDigits(const char str[], size_t size) {
  uint64_t sum = 0;
  if (!CheckInput(str, size)) {
    throw new std::invalid_argument("str");
  }

  for (size_t i = 0; str[i] != '\0' && i < size; i++) {
    const auto num = CharToNum(str[i]);
    sum += num;
  }

  return sum;
}

// CharToNumTestCase represents a unit test case. It contains a character to be
// tested and the expected output of the CharToNum function.
struct CharToNumTestCase {
  // The char to be tested
  char example;
  // The expected output of CharToNum
  uint8_t expected_number;
};

// CheckInputTestCase represents a unit test case. It contains a character to be
// tested and the expected output of the CheckInput function.
struct CheckInputTestCase {
  // The char to be tested
  char example;
  // The expected output of CheckInput
  bool expected;
};

// SumDigitsTestCase represents a unit test case. It contains a string to be
// tested and the expected output of the SumDigits function.
struct SumDigitsTestCase {
  // The string to be tested
  std::string example;
  // The expected output of CheckInput
  uint64_t expected;
};

bool RunUnitTests() {
  static const CharToNumTestCase CHAR_TO_NUM_CASES[] = {
      {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4},
      {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}};

  bool testsPassed = true;

  for (auto test_case : CHAR_TO_NUM_CASES) {
    const uint8_t num = CharToNum(test_case.example);
    if (num != test_case.expected_number) {
      std::cout << "Unit test failed: '" << test_case.example << "': Expected "
                << test_case.expected_number << ", got " << num << std::endl;
      testsPassed = false;
    }
  }

  static const CheckInputTestCase CHECK_INPUT_CASES[] = {
      {'0', true},  {'1', true},  {'2', true},  {'3', true},  {'4', true},
      {'5', true},  {'6', true},  {'7', true},  {'8', true},  {'9', true},
      {'a', false}, {'z', false}, {'_', false}, {'*', false},
  };

  for (auto test_case : CHECK_INPUT_CASES) {
    const bool is_number = CheckInput(test_case.example);
    if (is_number != test_case.expected) {
      std::cout << "Unit test failed: '" << test_case.example << "': Expected "
                << test_case.expected << ", got " << is_number << std::endl;
      testsPassed = false;
    }
  }

  static const SumDigitsTestCase SUM_DIGITS_CASES[] = {
      {"0", 0},  {"1", 1},  {"2", 2},  {"3", 3},  {"4", 4},   {"5", 5},
      {"6", 6},  {"7", 7},  {"8", 8},  {"9", 9},  {"00", 0},  {"01", 1},
      {"02", 2}, {"03", 3}, {"04", 4}, {"05", 5}, {"06", 6},  {"07", 7},
      {"08", 8}, {"09", 9}, {"11", 2}, {"12", 3}, {"13", 4},  {"14", 5},
      {"15", 6}, {"16", 7}, {"17", 8}, {"18", 9}, {"19", 10},
  };

  for (auto test_case : SUM_DIGITS_CASES) {
    const uint64_t sum =
        SumDigits(test_case.example.c_str(), test_case.example.length() + 1);
    if (sum != test_case.expected) {
      std::cout << "Unit test failed: '" << test_case.example << "': Expected "
                << test_case.expected << ", got " << sum << std::endl;
      testsPassed = false;
    }
  }

  return testsPassed;
}
}  // namespace digitsum
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool runUnitTests;
  if (!mjohnson::common::ParseArgs(argc, argv, &runUnitTests)) {
    return 1;
  }

  if (runUnitTests) {
    const bool result = mjohnson::digitsum::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  mjohnson::digitsum::Run();
  return 0;
}
