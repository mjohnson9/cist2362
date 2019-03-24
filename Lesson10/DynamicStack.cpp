// Copyright 2019 Michael Johnson

#include <cstddef>    // for size_t
#include <cstdint>    // for int32_t, int64_t
#include <iostream>   // for operator<<, endl, basic_ostream
#include <stdexcept>  // for length_error
#include <string>     // for allocator, operator+, to_string

#include "../common.h"  // for ParseArgs, RequestContinue, RequestContinue

namespace mjohnson {
namespace dynamicstack {

// FORWARD DECLARATIONS

/**
 * Creates a dynamic stack. This stack grows and shrinks to accomodate any
 * number of elements, up to the limits of allocatable memory.
 */
template <typename T>
class DynamicStack {
 private:
  /**
   * Item represents a single stack item. It's necessary to maintain the linked
   * list-like behavior of the stack.
   */
  struct Item {
    Item* next;
    T value;
  };

  /**
   * The item on the top of the stack. All items underneath can be found by
   * accessing this item's next value.
   */
  Item* top_;

 public:
  /**
   * Constructs a dynamic stack.
   */
  DynamicStack();
  /**
   * Destructs a dynamic stack.
   */
  ~DynamicStack();

  /**
   * Pushes a value onto the top of the stack.
   * @param value The value to push onto the top of the stack.
   */
  void Push(T value);
  /**
   * Pops a value off of the top of the stack.
   * @return The topmost value on the stack.
   */
  T Pop();

  /**
   * Retrieves the current size of the stack.
   * @return The size of the stack.
   */
  size_t Size();
  /**
   * Checks whether the stack is empty.
   * @return True if the stack is empty, false otherwise.
   */
  bool IsEmpty();
};

// MAIN FUNCTIONS

/**
 * Runs the demo program for the dynamic stack.
 * @return The exit status code.
 */
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    DynamicStack<int64_t> stack;

    for (size_t i = 0;; i++) {
      auto value = mjohnson::common::RequestInput<int64_t>(
          "What value would you like for item #" + std::to_string(i + 1) +
              "? (Enter -1 to stop entering values) ",
          nullptr);

      if (value == -1) {
        break;
      }

      stack.Push(value);
    }

    std::cout << std::endl << "Unwinding your stack:" << std::endl;
    for (size_t i = stack.Size() - 1; i >= 0; i--) {
      std::cout << "[" << (i + 1) << "]: " << stack.Pop() << std::endl;

      if (i == 0) {
        // If we don't do this, i will underflow and the for loop will continue
        // with i=18446744073709551615
        break;
      }
    }
    std::cout << std::endl;
  } while (mjohnson::common::RequestContinue());
  return 0;
}

// FUNCTION DEFINITIONS

template <typename T>
DynamicStack<T>::DynamicStack() {
  this->top_ = nullptr;
}

template <typename T>
DynamicStack<T>::~DynamicStack() {
  auto item = this->top_;
  while (item != nullptr) {
    auto next_item = item->next;
    delete item;
    item = next_item;
  }
}

template <typename T>
void DynamicStack<T>::Push(T value) {
  auto new_item = new DynamicStack<T>::Item();
  new_item->next = this->top_;
  new_item->value = value;

  this->top_ = new_item;
}

template <typename T>
T DynamicStack<T>::Pop() {
  if (this->IsEmpty()) {
    throw std::length_error("stack is empty");
  }

  auto old_top = this->top_;
  T value = old_top->value;

  this->top_ = old_top->next;

  delete old_top;

  return value;
}

template <typename T>
bool DynamicStack<T>::IsEmpty() {
  return (this->top_ == nullptr);
}

template <typename T>
size_t DynamicStack<T>::Size() {
  size_t size = 0;

  auto item = this->top_;
  while (item != nullptr) {
    size += 1;

    auto old_item = item;
    item = old_item->next;
  }

  return size;
}

// UNIT TESTING

/**
 * Runs the program's unit tests. Returns the success or failure of those unit
 * tests.
 * @return True if all unit tests passed, false otherwise.
 */
bool RunUnitTests() {
  bool test_return = true;

  {
    // Test empty stack pop
    DynamicStack<bool> test_stack;
    try {
      test_stack.Pop();
      std::cerr << "FAIL: Empty stack pop." << std::endl;

      test_return = false;  // NOLINT(clang-analyzer-deadcode.DeadStores)
    } catch (const std::length_error& ex) {
      std::cout << "PASS: Empty stack pop." << std::endl;
    }
  }

  {
    // Test retrieving a value
    DynamicStack<int32_t> test_stack;
    const int32_t kExpectedValue = 5;

    test_stack.Push(kExpectedValue);

    const int32_t received_value = test_stack.Pop();
    if (received_value == kExpectedValue) {
      std::cout << "PASS: Stack push-pop operation." << std::endl;
    } else {
      std::cerr << "FAIL: Stack push-pop operation: expected " << kExpectedValue
                << ", received " << received_value << std::endl;

      test_return = false;
    }
  }

  {
    // Test retrieving multiple values
    const size_t kValueMultiplier = 5;
    const size_t kNumValues = 10;

    DynamicStack<size_t> test_stack;

    for (size_t i = 0; i < kNumValues; i++) {
      test_stack.Push(i * kValueMultiplier);
    }

    bool test_passed = true;
    for (size_t i = (kNumValues - 1); i >= 0; i--) {
      const size_t expected_value = i * kValueMultiplier;
      const size_t received_value = test_stack.Pop();

      if (received_value != expected_value) {
        std::cerr << "FAIL: Stack multiple push-pop operation: expected "
                  << expected_value << ", received " << received_value
                  << std::endl;

        test_return = false;
        test_passed = false;
      }

      if (i == 0) {
        // If we don't do this, i will underflow and the for loop will continue
        // with i=18446744073709551615
        break;
      }
    }

    if (test_passed) {
      std::cout << "PASS: Stack multiple push-pop operation." << std::endl;
    }
  }

  return test_return;
}
}  // namespace dynamicstack
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::dynamicstack::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::dynamicstack::Run();
}

// Grade: 100
