// Copyright 2019 Michael Johnson

#ifdef USE_GMP
#include <gmp.h>
#include <gmpxx.h>
#endif  // USE_GMP

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
#endif  // USE_GMP

// FORWARD DECLARATIONS
// CalculateFactorial calculates the factorial of n
bigint CalculateFactorial(const bigint& n);
// ValidateFactorial validates a user input factorial request
bool ValidateFactorial(uint32_t n);

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    const auto n =
        mjohnson::common::RequestInput<uint32_t>("n = ", ValidateFactorial);

    const bigint n_converted(n);

    const std::chrono::high_resolution_clock::time_point begin =
        std::chrono::high_resolution_clock::now();
    const bigint result = CalculateFactorial(n_converted);
    const std::chrono::high_resolution_clock::time_point end =
        std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double> execution_seconds = end - begin;

    std::cout << n << "! = " << result << std::endl
              << "Executed in "
              << mjohnson::common::GetTimeString(execution_seconds) << "."
              << std::endl;
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UTILITY FUNCTIONS
bigint CalculateFactorial(const bigint& n) {
  if (n == 0 || n == 1) {
    return 1;
  }

  static std::map<bigint, bigint> result_table;

  auto initial_lookup = result_table[n];
  if (initial_lookup != 0) {
    // Shortcut: Result is already stored in lookup table
    return initial_lookup;
  }

  bigint i = n - 1;  // Start at n - 1, since we already checked n
  bigint result = 1;
  for (; i > 0; i--) {
    const bigint lookup_result = result_table[i];
    if (lookup_result == 0) {
      // The result isn't in the table; skip this iteration
      continue;
    }

    // We found the result; store it and break
    result = lookup_result;
    break;
  }

  // No cached results were found
  if (i == 0) {
    // We can't handle i = 0 because 0 * 1 = 0, so all results would be 0. Skip
    // 0 and fill with known beginning of i = 1, result = 1
    i = 1;
    result = 1;
  }

  // Calculate i + 1 through n, which are the remaining factorials
  for (bigint j = i + 1; j <= n; j++) {
    result *= j;
    // Store the result in the hash map
    result_table[j] = result;
  }

  return result;
}

bool ValidateFactorial(uint32_t n) {
#ifdef USE_GMP
  if (n >= 100000) {
    std::stringstream prompt_stream;
    prompt_stream
        << n
        << "! will take a long time to calculate and use more than 8GB of RAM. "
           "Are you sure that you would like to continue? [y/N] ";
    const std::string prompt = prompt_stream.str();

    const bool should_continue = mjohnson::common::RequestContinue(prompt);
    return should_continue;
  }
#else
  if (n > 20) {
    std::cout << "Cannot calculate factorials greater than 20 because it would "
                 "overflow a 64-bit unsigned integer."
              << std::endl
              << std::endl;
    return false;
  }
#endif  // USE_GMP

  return true;
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() {
  const size_t NUM_RESULTS = 21;
  static const bigint expected_result[NUM_RESULTS] = {1,
                                                      1,
                                                      2,
                                                      6,
                                                      24,
                                                      120,
                                                      720,
                                                      5040,
                                                      40320,
                                                      362880,
                                                      3628800,
                                                      39916800,
                                                      479001600,
                                                      6227020800,
                                                      87178291200,
                                                      1307674368000,
                                                      20922789888000,
                                                      355687428096000,
                                                      6402373705728000,
                                                      121645100408832000,
                                                      2432902008176640000};

  bool test_result = true;

  for (size_t i = 0; i < NUM_RESULTS; i++) {
    const bigint result = CalculateFactorial(i);
    const bigint expected = expected_result[i];
    if (result != expected) {
      std::cout << "FAIL: " << i << "!: Expected " << expected << ", got "
                << result << std::endl;
      test_result = false;
    }
  }

  return test_result;
}

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
