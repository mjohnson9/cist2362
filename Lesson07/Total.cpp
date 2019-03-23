// Copyright 2019 Michael Johnson

#include <cmath>
#include <iostream>

#include "../common.h"

namespace mjohnson {
namespace total {

// FORWARD DECLARATIONS
// Total asks the user for n values and then returns the total of the values
// that the user provided.
template <class T>
T Total(T n);
// ValidateNotNegative validates that input by the user is not negative. It
// displays an error message and returns false if the value is negative. It
// returns true otherwise.
bool ValidateInt64NotNegative(int64_t n);
bool ValidateDoubleNotNegative(double n);
// Unfortunately, these functions can't be a template. The compiler won't infer
// the type when it's passed as a reference to another function. In fact, it
// can't even handle overloads.

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    {
      mjohnson::common::ClearScreen();

      const auto n = mjohnson::common::RequestInput<int64_t>(
          "How many numbers would you like to total? ",
          ValidateInt64NotNegative);

      auto total = Total(n);
      std::cout << "The total of the numbers you entered is: " << total
                << std::endl
                << std::endl;
    }

    {
      const auto n = mjohnson::common::RequestInput<double>(
          "How many decimal numbers would you like to total? ",
          ValidateDoubleNotNegative);

      auto total = Total(n);
      std::cout << "The total of the numbers you entered is: " << total
                << std::endl
                << std::endl;
    }
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UTILITY FUNCTIONS
template <class T>
T Total(T n) {
  T runningTotal = 0;

  for (T i = 0; i < n;  // NOLINT(clang-analyzer-security.FloatLoopCounter)
       i++) {
    auto userValue = mjohnson::common::RequestInput<T>(
        "Enter a number to be added to the total: ", nullptr);
    runningTotal += userValue;
  }

  return runningTotal;
}

bool ValidateInt64NotNegative(int64_t n) {
  if (std::signbit(n)) {
    // signbit returns true when a number is negative
    std::cout << "You cannot total a negative number of numbers." << std::endl
              << std::endl;
    return false;
  }
  if (n < 2) {
    std::cout << "You must total at least 2 numbers." << std::endl << std::endl;
    return false;
  }

  return true;
}

bool ValidateDoubleNotNegative(double n) {
  if (std::signbit(n)) {
    // signbit returns true when a number is negative
    std::cout << "You cannot total a negative number of numbers." << std::endl
              << std::endl;
    return false;
  }
  if (n < 2) {
    std::cout << "You must total at least 2 numbers." << std::endl << std::endl;
    return false;
  }

  return true;
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() {
  bool success = true;

  return success;
}
}  // namespace total
}  // namespace mjohnson

int main(int argc, char *argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::total::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::total::Run();
}

// Grade: 100
