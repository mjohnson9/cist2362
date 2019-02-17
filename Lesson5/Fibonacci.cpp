// Copyright 2019 Michael Johnson

#ifdef USE_GMP
#include <gmp.h>
#include <gmpxx.h>
#endif

#include <chrono>  // NOLINT(build/c++11)
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

#include "../common.h"

namespace mjohnson {
namespace circle {

#ifdef USE_GMP
using bigint = mpz_class;
#else
using bigint = uint64_t;
#endif

// FORWARD DECLARATIONS
// CalculateFactorial calculates the factorial of n
bigint CalculateFibonacci(const bigint& n);
// ValidateFibonacci validates user input and limits it to reasonable numbers
bool ValidateFibonacci(uint32_t n);
// GetTimeString gets a human-readable duration from a duration<double>
std::string GetTimeString(std::chrono::duration<double> duration);

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    const auto n =
        mjohnson::common::RequestInput<uint32_t>("n = ", ValidateFibonacci);

    const std::chrono::high_resolution_clock::time_point begin =
        std::chrono::high_resolution_clock::now();
    const bigint result = CalculateFibonacci(n);
    const std::chrono::high_resolution_clock::time_point end =
        std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double> execution_seconds = end - begin;

    std::cout << "Fibonacci(" << n << ") = " << result << std::endl
              << std::endl
              << "Executed in "
              << mjohnson::common::GetTimeString(execution_seconds) << "."
              << std::endl
              << std::endl;
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UTILITY FUNCTIONS
bigint CalculateFibonacci(const bigint& n) {
  // Known values
  if (n == 0) {
    return 0;
  }
  if (n == 1 || n == 2) {
    return 1;
  }

  // The maximum value of n that can be calculated at once. Used to prevent
  // stack overflows.
  static const uint32_t MAX_RECURSION = 1000;

  // result_table is an ordered hash map that persists across function calls. We
  // use it for memoization.
  static std::map<bigint, bigint> result_table;

  // Check if we already have the result stored
  auto initial_lookup = result_table[n];
  if (initial_lookup != 0) {
    // Shortcut: Result is already stored in lookup table
    return initial_lookup;
  }

  // If we're at a high enough n that a stack overflow is probable
  if (n > MAX_RECURSION) {
    // Find the largest sequence that has been calculated by using a binary
    // search
    bigint upper_end = n - 1;
    bigint lower_end = 3;
    while (lower_end < upper_end) {
      // Check is the midpoint between upper_end and lower_end
      bigint check = (upper_end + lower_end) >> 1;
      if (result_table[check] != 0) {
        lower_end = check + 1;
      } else {
        upper_end = check;
      }
    }

    lower_end -= 1;  // We always end up 1 above the highest calculated

    // lower_end is now the largest sequence calculated

    if ((n - lower_end) > MAX_RECURSION) {
      // If the largest calculated result is farther away than our max recursion
      // depth, we're likely to cause a stack overflow. Calculate MAX_RECURSION
      // results at a time and cache before continuing.
      CalculateFibonacci(n - MAX_RECURSION);
    }
  }

  // Calculate and store the result
  const bigint result = CalculateFibonacci(n - 1) + CalculateFibonacci(n - 2);
  result_table[n] = result;

  return result;
}

bool ValidateFibonacci(uint32_t n) {
  if (n >= 155000) {
    std::stringstream prompt_stream;
    prompt_stream << "Fibonacci(" << n << ") will use more than ";
    if (n >= 310000) {
      prompt_stream << "4GB";
    } else if (n >= 270000) {
      prompt_stream << "3GB";
    } else if (n >= 220000) {
      prompt_stream << "2GB";
    } else {
      prompt_stream << "1GB";
    }
    prompt_stream << " of RAM";
    if (n >= 310000) {
      prompt_stream << " and take a long time to calculate";
    }
    prompt_stream << ". Are you sure that you'd like to continue? [y/N] ";
    const bool should_continue =
        mjohnson::common::RequestContinue(prompt_stream.str());
    return should_continue;
  }
#ifndef USE_GMP
  if (n > 93) {
    std::cout
        << "This program cannot calculate Fibonacci sequences with n greater "
           "than 93 because it overflows a 64-bit unsigned integer."
        << std::endl
        << std::endl;
    return false;
  }
#endif

  return true;
}  // namespace circle

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or
// failure of those unit tests as a boolean.
bool RunUnitTests() { return true; }

}  // namespace circle
}  // namespace mjohnson

int main(int argc, char* argv[]) {
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
