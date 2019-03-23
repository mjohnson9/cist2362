// Copyright 2019 Michael Johnson

#include <iostream>

#include "../common.h"

namespace mjohnson {
namespace programname {

// FORWARD DECLARATIONS

// MAIN FUNCTIONS
int Run() {}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or
// failure of those unit tests as a boolean.
bool RunUnitTests() { return true; }
}  // namespace programname
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::programname::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::programname::Run();
}

// Grade: 100
