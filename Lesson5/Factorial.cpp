// Copyright 2019 Michael Johnson

#include <gmp.h>
#include <gmpxx.h>

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

#include "../common.h"

namespace mjohnson {
namespace circle {

// FORWARD DECLARATIONS
// CalculateFactorial calculates the factorial of n
mpz_class CalculateFactorial(const mpz_class& n);
// GetTimeString gets a human-readable duration from a duration<double>
std::string GetTimeString(std::chrono::duration<double> duration);

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    const auto n = mjohnson::common::RequestInput<int32_t>("n = ", NULL);
    const mpz_class n_bigint(n);

    const std::chrono::high_resolution_clock::time_point begin =
        std::chrono::high_resolution_clock::now();
    const mpz_class factorial = CalculateFactorial(n_bigint);
    const std::chrono::high_resolution_clock::time_point end =
        std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double> execution_seconds = end - begin;

    std::cout << "n! = " << factorial << std::endl
              << "Executed in " << GetTimeString(execution_seconds) << "."
              << std::endl;
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UTILITY FUNCTIONS
mpz_class CalculateFactorial(const mpz_class& n) {
  static std::map<mpz_class, mpz_class> result_table;

  {
    const mpz_class& initial_lookup = result_table[n];
    if (initial_lookup != 0) {
      // Shortcut: Result is already stored in lookup table
      return initial_lookup;
    }
  }

  mpz_class i = n;
  mpz_class result = 1;
  for (; i > 0; i--) {
    const mpz_class& lookup_result = result_table[i];
    if (lookup_result == 0) {
      // The result isn't in the table; skip this iteration
      continue;
    }

    // We found the result; store it and break
    result = lookup_result;
    break;
  }

  for (mpz_class j = i + 1; j <= n; j++) {
    result *= j;
    result_table[j] = result;
  }

  return result;
}

std::string GetTimeString(std::chrono::duration<double> duration_s) {
  std::stringstream result_stream;
  result_stream << std::fixed << std::setprecision(0);
  auto hours = std::chrono::duration_cast<std::chrono::hours>(duration_s);
  if (hours.count() > 1) {
    result_stream << hours.count() << "h";
    duration_s -= hours;
  }
  auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration_s);
  if (minutes.count() > 1) {
    result_stream << minutes.count() << "m";
    duration_s -= minutes;
  }
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration_s);
  if (seconds.count() > 1) {
    result_stream << seconds.count() << "s";
    duration_s -= seconds;
  }
  auto milliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration_s);
  if (milliseconds.count() > 1) {
    result_stream << milliseconds.count() << "ms";
    duration_s -= milliseconds;
  }
  auto microseconds =
      std::chrono::duration_cast<std::chrono::microseconds>(duration_s);
  if (microseconds.count() > 1) {
    result_stream << microseconds.count() << "us";
    duration_s -= microseconds;
  }
  auto nanoseconds =
      std::chrono::duration_cast<std::chrono::nanoseconds>(duration_s);
  if (nanoseconds.count() > 1) {
    result_stream << nanoseconds.count() << "ns";
  }

  return result_stream.str();
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
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
