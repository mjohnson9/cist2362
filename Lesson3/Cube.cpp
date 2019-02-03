// Copyright 2019 Michael Johnson

#include <iostream>

#include "../common.h"

namespace mjohnson {
namespace salesdata {

class Cube {
 private:
  double width_ = 0l;
  double length_ = 0l;
  double height_ = 0l;

 public:
  Cube() = default;

  double Width() const { return this->width_; }
  void SetWidth(double width) { this->width_ = width; }

  double Length() const { return this->length_; }
  void SetLength(double length) { this->length_ = length; }

  double Height() const { return this->height_; }
  void SetHeight(double height) { this->height_ = height; }

  double Volume() const {
    return this->Width() * this->Length() * this->Height();
  }
};

bool ValidateDimension(double dimension);

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    mjohnson::common::ClearScreen();

    Cube cube;

    cube.SetHeight(mjohnson::common::RequestInput<double>(
        "Enter the cube's height: ", ValidateDimension));
    cube.SetWidth(mjohnson::common::RequestInput<double>(
        "Enter the cube's width: ", ValidateDimension));
    cube.SetLength(mjohnson::common::RequestInput<double>(
        "Enter the cube's length: ", ValidateDimension));

    mjohnson::common::ClearScreen();

    std::cout << "Cube:" << std::endl
              << "| Height: " << cube.Height() << std::endl
              << "| Width: " << cube.Width() << std::endl
              << "| Length: " << cube.Length() << std::endl
              << "| Volume: " << cube.Volume() << std::endl
              << std::endl;
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() {
  bool success = true;

  Cube test;

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
  if (test.Height() != 0) {
    std::cout << "FAIL: Expected no-arg constructor to have height 0, got "
              << test.Height() << std::endl;
    success = false;
  }
  if (test.Volume() != 0) {
    std::cout << "FAIL: Expected no-arg constructor to have volume 0, got "
              << test.Volume() << std::endl;
    success = false;
  }

  const double test_dimensions = 2l;
  test.SetLength(test_dimensions);
  test.SetWidth(test_dimensions);
  test.SetHeight(test_dimensions);

  if (test.Width() != test_dimensions) {
    std::cout << "FAIL: Expected SetWidth(" << test_dimensions
              << ") to cause Width() to return " << test_dimensions << ", got "
              << test.Width() << std::endl;
    success = false;
  }
  if (test.Length() != test_dimensions) {
    std::cout << "FAIL: Expected SetLength(" << test_dimensions
              << ") to cause Length() to return " << test_dimensions << ", got "
              << test.Length() << std::endl;
    success = false;
  }
  if (test.Height() != test_dimensions) {
    std::cout << "FAIL: Expected SetHeight(" << test_dimensions
              << ") to cause Height() to return " << test_dimensions << ", got "
              << test.Height() << std::endl;
    success = false;
  }
  if (test.Volume() != (test_dimensions * test_dimensions * test_dimensions)) {
    std::cout << "FAIL: Expected area() to return "
              << (test_dimensions * test_dimensions * test_dimensions)
              << ", got " << test.Volume() << std::endl;
    success = false;
  }

  return success;
}

bool ValidateDimension(double dimension) {
  if (dimension < 0) {
    std::cout << "The cube can not have negative dimensions." << std::endl
              << std::endl;
    return false;
  }
  if (dimension == 0) {
    std::cout << "The cube can not have a dimension of 0." << std::endl
              << std::endl;
    return false;
  }
  return true;
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
