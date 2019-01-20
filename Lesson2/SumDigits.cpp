// Copyright 2019 Michael Johnson

#include <iostream>

#include "../common.h"

namespace mjohnson {
namespace digitsum {
// CheckInput checks if the input char is a number or not
bool IsNumber(char c);
// ValidateInputString checks if all of the input characters are numbers or not.
// It outputs an error message on cout for the first character that isn't.
bool ValidateInputString(std::string str);
// CharToNum converts a char to an unsigned integer. It throws a
// std::invalid_argument exception if the input is not a number.
uint8_t CharToNum(char c);
// SumDigits sums all of the digits in a char array. It throws a
// std::invalid_argument exception if the input contains non-digits.
uint64_t SumDigits(const char str[], size_t size);

void Run() {
  do {
    mjohnson::common::ClearScreen();

    const std::string str = mjohnson::common::RequestInput<std::string>(
        "Please enter a string of digits to have them summed: ",
        ValidateInputString);

    mjohnson::common::ClearScreen();

    const uint64_t sum = SumDigits(str.c_str(), str.length() + 1);
    std::cout << "The sum of the digits in \"" << str << "\" is " << sum << "."
              << std::endl
              << std::endl;
  } while (mjohnson::common::RequestContinue());
}

uint8_t CharToNum(const char c) {
  if (!IsNumber(c)) {
    throw std::invalid_argument("c");
  }
  return (9 - ('9' - c));
}

bool IsNumber(const char c) { return (c >= '0' && c <= '9'); }

bool ValidateInputString(std::string str) {
  bool all_digits = true;
  for (auto const& c : str) {
    if (!IsNumber(c)) {  // The given character isn't a number
      std::cout << "\"" << c << "\" is not a digit. ";
      all_digits = false;
    }
  }

  if (!all_digits) {
    std::cout << "Please enter only digits." << std::endl << std::endl;
  }

  return all_digits;
}

uint64_t SumDigits(const char str[], size_t size) {
  uint64_t sum = 0;

  for (size_t i = 0; str[i] != '\0' && i < size; i++) {
    const char c = str[i];

    if (!IsNumber(c)) {  // Throw an exception if this character isn't a number
      throw std::invalid_argument("str");
    }

    const auto num = CharToNum(c);
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

// IsNumberTestCase represents a unit test case. It contains a character to be
// tested and the expected output of the IsNumber function.
struct IsNumberTestCase {
  // The char to be tested
  char example;
  // The expected output of IsNumber
  bool expected;
};

// SumDigitsTestCase represents a unit test case. It contains a string to be
// tested and the expected output of the SumDigits function.
struct SumDigitsTestCase {
  // The string to be tested
  std::string example;
  // The expected output of SumDigits
  uint64_t expected;
};

bool RunUnitTests() {
  static const CharToNumTestCase char_to_num_cases[] = {
      {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4},
      {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}};

  bool tests_passed = true;

  for (auto const& test_case : char_to_num_cases) {
    const uint8_t num = CharToNum(test_case.example);
    if (num != test_case.expected_number) {
      std::cout << "Unit test failed: '" << test_case.example << "': Expected "
                << test_case.expected_number << ", got " << num << std::endl;
      tests_passed = false;
    }
  }

  static const IsNumberTestCase is_number_cases[] = {
      {'0', true},  {'1', true},  {'2', true},  {'3', true},  {'4', true},
      {'5', true},  {'6', true},  {'7', true},  {'8', true},  {'9', true},
      {'a', false}, {'z', false}, {'_', false}, {'*', false},
  };

  for (auto const& test_case : is_number_cases) {
    const bool is_number = IsNumber(test_case.example);
    if (is_number != test_case.expected) {
      std::cout << "Unit test failed: '" << test_case.example << "': Expected "
                << test_case.expected << ", got " << is_number << std::endl;
      tests_passed = false;
    }
  }

  static const SumDigitsTestCase sum_digits_cases[] = {
      {"0", 0},  {"1", 1},  {"2", 2},  {"3", 3},  {"4", 4},   {"5", 5},
      {"6", 6},  {"7", 7},  {"8", 8},  {"9", 9},  {"00", 0},  {"01", 1},
      {"02", 2}, {"03", 3}, {"04", 4}, {"05", 5}, {"06", 6},  {"07", 7},
      {"08", 8}, {"09", 9}, {"11", 2}, {"12", 3}, {"13", 4},  {"14", 5},
      {"15", 6}, {"16", 7}, {"17", 8}, {"18", 9}, {"19", 10}, {"2514", 12}};

  for (auto const& test_case : sum_digits_cases) {
    const uint64_t sum =
        SumDigits(test_case.example.c_str(), test_case.example.length() + 1);
    if (sum != test_case.expected) {
      std::cout << "Unit test failed: '" << test_case.example << "': Expected "
                << test_case.expected << ", got " << sum << std::endl;
      tests_passed = false;
    }
  }

  return tests_passed;
}
}  // namespace digitsum
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
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
