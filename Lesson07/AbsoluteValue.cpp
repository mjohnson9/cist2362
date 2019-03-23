// Copyright 2019 Michael Johnson

#include <cmath>
#include <iostream>

#include "../common.h"

namespace mjohnson {
namespace absolutevalue {

// FORWARD DECLARATIONS
// AbsoluteValue returns the absolute value of the variable passed. T must be a
// type which can be compared and implements the unary - operator.
template <class T>
T AbsoluteValue(T n);

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    mjohnson::common::ClearScreen();

    const auto nInt64 =
        mjohnson::common::RequestInput<int64_t>("Enter a number: ", nullptr);
    std::cout << "AbsoluteValue(" << nInt64 << ") = " << AbsoluteValue(nInt64)
              << std::endl
              << std::endl;

    const auto nDouble = mjohnson::common::RequestInput<double>(
        "Enter a decimal number: ", nullptr);
    std::cout << "AbsoluteValue(" << nDouble << ") = " << AbsoluteValue(nDouble)
              << std::endl
              << std::endl;
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UTILITY FUNCTIONS
template <class T>
T AbsoluteValue(T n) {
  // signbit returns true when n is negative, including negative zero, negative
  // infinity, and negative NaNs. If signbit returns true, return the sign
  // inverse (unary -) of n.
  return std::signbit(n) ? -n : n;
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() {
  bool success = true;

  {
    // Test a positive integer
    const int32_t testValue = 1;
    const int32_t expected = 1;
    const int32_t result = AbsoluteValue(testValue);
    if (result != expected) {
      std::cout << "FAIL: AbsoluteValue(" << testValue << "), expected "
                << expected << ", got " << result << std::endl;
      success = false;
    }
  }

  {
    // Test a negative integer
    const int32_t testValue = -1;
    const int32_t expected = 1;
    const int32_t result = AbsoluteValue(testValue);
    if (result != expected) {
      std::cout << "FAIL: AbsoluteValue(" << testValue << "), expected "
                << expected << ", got " << result << std::endl;
      success = false;
    }
  }

  {
    // Test a positive double
    const double testValue = 1.5l;
    const double expected = 1.5l;
    const double result = AbsoluteValue(testValue);
    if (result != expected) {
      std::cout << "FAIL: AbsoluteValue(" << testValue << "), expected "
                << expected << ", got " << result << std::endl;
      success = false;
    }
  }

  {
    // Test a negative double
    const double testValue = -1.5l;
    const double expected = 1.5l;
    const double result = AbsoluteValue(testValue);
    if (result != expected) {
      std::cout << "FAIL: AbsoluteValue(" << testValue << "), expected "
                << expected << ", got " << result << std::endl;
      success = false;
    }
  }

  {
    // Test with an unsigned type
    const uint32_t testValue = 1;
    const uint32_t expected = 1;
    const uint32_t result = AbsoluteValue(testValue);
    if (result != expected) {
      std::cout << "FAIL: AbsoluteValue(" << testValue << "), expected "
                << expected << ", got " << result << std::endl;
      success = false;
    }
  }

  return success;
}
}  // namespace absolutevalue
}  // namespace mjohnson

int main(int argc, char *argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::absolutevalue::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::absolutevalue::Run();
}

// Grade: 100
