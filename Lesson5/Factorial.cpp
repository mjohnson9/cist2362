// Copyright 2019 Michael Johnson

#include <iostream>

#include "../common.h"

namespace mjohnson {
namespace circle {

// FORWARD DECLARATIONS
// CalculateFactorial calculates the factorial of n
uint64_t CalculateFactorial(uint64_t n);

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    mjohnson::common::ClearScreen();

    const auto n = mjohnson::common::RequestInput<uint64_t>("n = ", NULL);

    uint64_t factorial = CalculateFactorial(n);

    std::cout << "n! = " << factorial << std::endl;
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UTILITY FUNCTIONS
uint64_t CalculateFactorial(uint64_t n) {
  if (n == 0) {
    return 1;
  }
  return n * CalculateFactorial(n - 1);
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() { return true; }

}  // namespace circle
}  // namespace mjohnson

int main(int argc, char *argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::circle::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::circle::Run();
}
