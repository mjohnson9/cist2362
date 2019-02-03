// Copyright 2019 Michael Johnson

#include <iomanip>
#include <iostream>

#include "../common.h"

namespace mjohnson {
namespace inventory {

class Inventory {
 private:
  int item_number_ = 0;
  int quantity_ = 0;
  double cost_ = 0;

 public:
  Inventory() = default;

  Inventory(int item_number, int quantity, double cost)
      : item_number_(item_number), quantity_(quantity), cost_(cost) {}

  void SetItemNumber(int item_number) { this->item_number_ = item_number; }
  int ItemNumber() const { return this->item_number_; }
  void SetQuantity(int quantity) { this->quantity_ = quantity; }
  int Quantity() const { return this->quantity_; }
  void SetCost(double cost) { this->cost_ = cost; }
  double Cost() const { return this->cost_; }
  double TotalCost() const {
    return static_cast<double>(this->quantity_) * this->cost_;
  }
};

// FORWARD DECLARATIONS
// ValidateNonNegative validates that a given input is not negative.
bool ValidateItemNumber(int item_number);
bool ValidateQuantity(int quantity);
bool ValidateCost(double cost);

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    mjohnson::common::ClearScreen();

    std::cout << "========== INVENTORY ==========" << std::endl;

    const auto item_number = mjohnson::common::RequestInput<int>(
        "Enter the item number: ", ValidateItemNumber);
    const auto quantity = mjohnson::common::RequestInput<int>(
        "Enter the quantity of the item: ", ValidateQuantity);
    const auto cost = mjohnson::common::RequestInput<double>(
        "Enter the cost of the item: $", ValidateCost);

    mjohnson::common::ClearScreen();

    Inventory inventory(item_number, quantity, cost);

    const std::streamsize starting_precision = std::cout.precision();

    std::cout << "========== INVENTORY ==========" << std::endl
              << "| Item number: " << inventory.ItemNumber() << std::endl
              << "| Quantity: " << inventory.Quantity() << std::endl
              << std::fixed << std::setprecision(2) << "| Cost: $"
              << inventory.Cost() << std::endl
              << "| Total cost: $" << inventory.TotalCost() << std::endl
              << std::endl;

    // Reset cout's formatting
    std::cout << std::setprecision(static_cast<int>(starting_precision));
    std::cout.unsetf(std::ios_base::floatfield);
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UTILITY FUNCTIONS
bool ValidateItemNumber(int item_number) {
  if (item_number < 0) {
    std::cout << "The item number cannot be negative." << std::endl
              << std::endl;
    return false;
  }

  return true;
}

bool ValidateQuantity(int quantity) {
  if (quantity < 0) {
    std::cout << "The quantity cannot be negative." << std::endl << std::endl;
    return false;
  }

  return true;
}

bool ValidateCost(double cost) {
  if (cost <= 0) {
    std::cout << "The cost must be greater than zero." << std::endl
              << std::endl;
    return false;
  }

  return true;
}

// UNIT TESTING

bool TestNoArgConstructor();
bool TestArgConstructor();
bool TestSetItemNumber();
bool TestSetQuantity();
bool TestSetCost();

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() {
  bool success = true;

  success = TestNoArgConstructor() && success;
  success = TestArgConstructor() && success;
  success = TestSetItemNumber() && success;
  success = TestSetQuantity() && success;
  success = TestSetCost() && success;

  return success;
}

bool TestNoArgConstructor() {
  bool success = true;

  Inventory test;

  if (test.ItemNumber() != 0) {
    std::cout << "FAIL: Expected no-arg constructor to have item number 0, got "
              << test.ItemNumber() << std::endl;
    success = false;
  }
  if (test.Quantity() != 0) {
    std::cout << "FAIL: Expected no-arg constructor to have quantity 0, got "
              << test.Quantity() << std::endl;
    success = false;
  }
  if (test.Cost() != 0) {
    std::cout << "FAIL: Expected no-arg constructor to have cost 0, got "
              << test.Cost() << std::endl;
    success = false;
  }
  if (test.TotalCost() != 0) {
    std::cout << "FAIL: Expected no-arg constructor to have total cost 0, got "
              << test.TotalCost() << std::endl;
    success = false;
  }

  return success;
}

bool TestArgConstructor() {
  bool success = true;

  constexpr int TEST_ITEM_NUMBER = 300;
  constexpr int TEST_QUANTITY = 500;
  constexpr double TEST_COST = 300.50;
  constexpr double TEST_TOTAL_COST =
      static_cast<double>(TEST_QUANTITY) * TEST_COST;

  Inventory test(TEST_ITEM_NUMBER, TEST_QUANTITY, TEST_COST);

  if (test.ItemNumber() != TEST_ITEM_NUMBER) {
    std::cout << "FAIL: Expected constructor to set item number to "
              << TEST_ITEM_NUMBER << ", got " << test.ItemNumber() << std::endl;
    success = false;
  }
  if (test.Quantity() != TEST_QUANTITY) {
    std::cout << "FAIL: Expected constructor to set quantity to "
              << TEST_QUANTITY << ", got " << test.Quantity() << std::endl;
    success = false;
  }
  if (test.Cost() != TEST_COST) {
    std::cout << "FAIL: Expected constructor to set cost to " << TEST_COST
              << ", got " << test.Cost() << std::endl;
    success = false;
  }
  if (test.TotalCost() != TEST_TOTAL_COST) {
    std::cout << "FAIL: Expected constructor to cause total cost to return "
              << TEST_TOTAL_COST << ", got " << test.TotalCost() << std::endl;
    success = false;
  }

  return success;
}

bool TestSetItemNumber() {
  Inventory test;

  constexpr int TEST_ITEM_NUMBER = 500;
  test.SetItemNumber(TEST_ITEM_NUMBER);

  if (test.ItemNumber() != TEST_ITEM_NUMBER) {
    std::cout << "FAIL: Expected SetItemNumber(" << TEST_ITEM_NUMBER
              << ") cause ItemNumber() to return " << TEST_ITEM_NUMBER
              << ", got " << test.ItemNumber() << std::endl;
    return false;
  }

  return true;
}

bool TestSetQuantity() {
  Inventory test;

  constexpr int TEST_QUANTITY = 500;
  test.SetQuantity(TEST_QUANTITY);

  if (test.Quantity() != TEST_QUANTITY) {
    std::cout << "FAIL: Expected SetQuantity(" << TEST_QUANTITY
              << ") cause Quantity() to return " << TEST_QUANTITY << ", got "
              << test.Quantity() << std::endl;
    return false;
  }

  return true;
}

bool TestSetCost() {
  Inventory test;

  constexpr double TEST_COST = 500.50;
  test.SetCost(TEST_COST);

  if (test.Cost() != TEST_COST) {
    std::cout << "FAIL: Expected SetCost(" << TEST_COST
              << ") cause Cost() to return " << TEST_COST << ", got "
              << test.Cost() << std::endl;
    return false;
  }

  return true;
}

}  // namespace inventory
}  // namespace mjohnson

int main(int argc, char *argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::inventory::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::inventory::Run();
}
