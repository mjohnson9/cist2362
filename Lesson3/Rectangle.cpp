// Copyright 2019 Michael Johnson

#include <iostream>

#include "../common.h"

namespace mjohnson {
namespace salesdata {

class Rectangle {
 private:
  double width_ = 0l;
  double length_ = 0l;

 public:
  Rectangle() = default;

  double Width() const { return this->width_; }
  void SetWidth(double width) { this->width_ = width; }

  double Length() const { return this->length_; }
  void SetLength(double length) { this->length_ = length; }

  double Area() const { return this->Width() * this->Length(); }
};

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  std::cout << "User interaction not implemented." << std::endl;

  return 0;
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() {
  bool success = true;

  Rectangle test;

  if (test.Width() != 0) {
    std::cout << "FAIL: Expected no-arg constructor to have width 0, got "
              << test.Width() << std::endl;
    success = false;
  }
  if (test.Length() != 0) {
    std::cout << "FAIL: Expected no-arg constructor to have length 0, got "
              << test.Length() << std::endl;
    success = false;
  }
  if (test.Area() != 0) {
    std::cout << "FAIL: Expected no-arg constructor to have area 0, got "
              << test.Area() << std::endl;
    success = false;
  }

  const double test_dimensions = 2l;
  test.SetLength(test_dimensions);
  test.SetWidth(test_dimensions);

  if (test.Width() != test_dimensions) {
    std::cout << "FAIL: Expected set_width(" << test_dimensions
              << ") to cause Width() to return " << test_dimensions << ", got "
              << test.Width() << std::endl;
    success = false;
  }
  if (test.Length() != test_dimensions) {
    std::cout << "FAIL: Expected set_length(" << test_dimensions
              << ") to cause Length() to return " << test_dimensions << ", got "
              << test.Length() << std::endl;
    success = false;
  }
  if (test.Area() != (test_dimensions * test_dimensions)) {
    std::cout << "FAIL: Expected area() to return "
              << (test_dimensions * test_dimensions) << ", got " << test.Area()
              << std::endl;
    success = false;
  }

  return success;
}
}  // namespace salesdata
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::salesdata::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::salesdata::Run();
}
