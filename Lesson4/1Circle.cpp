// Copyright 2019 Michael Johnson

#include <iostream>

#include "../common.h"

namespace mjohnson {
namespace circle {

// Circle provides a representation of a simple circle
class Circle {
 private:
  // pi constant used for calculations
  static constexpr double PI = 3.14159;
  // the circle's radius
  double radius_ = 0l;

 public:
  // The default Circle constructor creates a Circle with a radius of 0
  Circle() = default;

  // The Circle(double) constructor creates a Circle with a radius of r
  explicit Circle(double r) : radius_(r) {}

  // SetRadius sets the radius of this Circle
  void SetRadius(double r) { this->radius_ = r; }
  double Radius() const { return this->radius_; }

  double Area() const { return Circle::PI * this->radius_ * this->radius_; }
  double Diameter() const { return this->radius_ * 2l; }
  double Circumference() const { return 2l * Circle::PI * this->radius_; }
};

// FORWARD DECLARATIONS
// ValidateRadius validates that the given radius r is acceptable input for the
// example program
bool ValidateRadius(double r);

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    mjohnson::common::ClearScreen();

    std::cout << "========== CIRCLE ==========" << std::endl;

    const auto radius = mjohnson::common::RequestInput<double>(
        "Enter the radius of the circle in feet: ", ValidateRadius);

    mjohnson::common::ClearScreen();

    Circle circle(radius);

    std::cout << "========== CIRCLE ==========" << std::endl
              << "| Radius: " << circle.Radius() << " ft" << std::endl
              << "| Diameter: " << circle.Diameter() << " ft" << std::endl
              << "| Circumference: " << circle.Circumference() << " ft"
              << std::endl
              << "| Area: " << circle.Area() << " sqft" << std::endl
              << std::endl;
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UTILITY FUNCTIONS
bool ValidateRadius(double r) {
  if (r <= 0) {
    std::cout << "The input radius must be greater than zero." << std::endl
              << std::endl;
    return false;
  }

  return true;
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() {
  bool success = true;

  Circle test;

  if (test.Radius() != 0) {
    std::cout << "FAIL: Expected no-arg constructor to have radius 0, got "
              << test.Radius() << std::endl;
    success = false;
  }
  if (test.Area() != 0) {
    std::cout << "FAIL: Expected no-arg constructor to have area 0, got "
              << test.Area() << std::endl;
    success = false;
  }
  if (test.Diameter() != 0) {
    std::cout << "FAIL: Expected no-arg constructor to have diameter 0, got "
              << test.Diameter() << std::endl;
    success = false;
  }
  if (test.Circumference() != 0) {
    std::cout
        << "FAIL: Expected no-arg constructor to have circumference 0, got "
        << test.Circumference() << std::endl;
    success = false;
  }

  constexpr double PI = 3.14159;
  constexpr double TEST_RADIUS = 2.5l;

  test.SetRadius(TEST_RADIUS);

  if (test.Radius() != TEST_RADIUS) {
    std::cout << "FAIL: Expected SetRadius(" << TEST_RADIUS
              << ") to cause Radius() to return " << TEST_RADIUS << ", got "
              << test.Radius() << std::endl;
    success = false;
  }

  constexpr double TEST_AREA = PI * TEST_RADIUS * TEST_RADIUS;
  if (test.Area() != TEST_AREA) {
    std::cout << "FAIL: Expected Area() to return " << TEST_AREA << ", got "
              << test.Area() << std::endl;
    success = false;
  }

  constexpr double TEST_DIAMETER = TEST_RADIUS * 2l;
  if (test.Diameter() != TEST_DIAMETER) {
    std::cout << "FAIL: Expected Diameter() to return " << TEST_DIAMETER
              << ", got " << test.Diameter() << std::endl;
    success = false;
  }

  constexpr double TEST_CIRCUMFERENCE = 2l * PI * TEST_RADIUS;
  if (test.Circumference() != TEST_CIRCUMFERENCE) {
    std::cout << "FAIL: Expected Circumference() to return "
              << TEST_CIRCUMFERENCE << ", got " << test.Circumference()
              << std::endl;
    success = false;
  }

  return success;
}
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
