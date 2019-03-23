// Copyright 2019 Michael Johnson

#include <iostream>
#include <stdexcept>

#include "../common.h"

namespace mjohnson {
namespace staticstack {

// FORWARD DECLARATIONS

template <typename T>
class StaticStack {
 private:
  T* stack_;
  size_t stack_size_;
  size_t stack_capacity_;

 public:
  explicit StaticStack(size_t capacity);
  ~StaticStack() { delete[] this->stack_; }

  void Push(T value);
  T Pop();

  size_t Size();
  size_t Capacity();
  bool IsFull();
  bool IsEmpty();
};

// MAIN FUNCTIONS
int Run() {
  do {
    auto stack_capacity = mjohnson::common::RequestInput<size_t>(
        "How many items would you like to put on the stack? ", nullptr);
    StaticStack<int64_t> stack(stack_capacity);

    for (size_t i = 0; i < stack.Capacity(); i++) {
      auto value = mjohnson::common::RequestInput<int64_t>(
          "What value would you like for item #" + std::to_string(i + 1) + "? ",
          nullptr);
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
  } while (mjohnson::common::RequestContinue());
  return 0;
}

// FUNCTION DEFINITIONS

template <typename T>
StaticStack<T>::StaticStack(size_t capacity) {
  if (capacity <= 0) {
    throw std::invalid_argument("capacity must be greater than 0");
  }

  this->stack_ = new T[capacity];
  this->stack_size_ = 0;
  this->stack_capacity_ = capacity;
}

template <typename T>
void StaticStack<T>::Push(T value) {
  if (this->IsFull()) {
    throw std::length_error("stack is full");
  }

  this->stack_[this->stack_size_] = value;
  this->stack_size_ += 1;
}

template <typename T>
T StaticStack<T>::Pop() {
  if (this->IsEmpty()) {
    throw std::length_error("stack is empty");
  }

  T value = this->stack_[this->stack_size_ - 1];
  this->stack_size_ -= 1;

  return value;
}

template <typename T>
bool StaticStack<T>::IsFull() {
  return (this->stack_size_ == this->stack_capacity_);
}

template <typename T>
bool StaticStack<T>::IsEmpty() {
  return (this->stack_size_ == 0);
}

template <typename T>
size_t StaticStack<T>::Size() {
  return this->stack_size_;
}

template <typename T>
size_t StaticStack<T>::Capacity() {
  return this->stack_capacity_;
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or
// failure of those unit tests as a boolean.
bool RunUnitTests() {
  bool test_return = true;

  {
    // Test empty stack pop
    StaticStack<bool> test_stack(1);
    try {
      test_stack.Pop();
      std::cerr << "FAIL: Empty stack pop." << std::endl;

      test_return = false;  // NOLINT(clang-analyzer-deadcode.DeadStores)
    } catch (const std::length_error& ex) {
      std::cout << "PASS: Empty stack pop." << std::endl;
    }
  }

  {
    // Test full stack push
    StaticStack<bool> test_stack(1);
    try {
      test_stack.Push(true);
      test_stack.Push(true);
      std::cerr << "FAIL: Full stack push." << std::endl;

      test_return = false;
    } catch (const std::length_error& ex) {
      std::cout << "PASS: Full stack push." << std::endl;
    }
  }

  {
    // Test retrieving a value
    StaticStack<int32_t> test_stack(1);
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

    StaticStack<size_t> test_stack(kNumValues);

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
}  // namespace staticstack
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::staticstack::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::staticstack::Run();
}

// Grade: 100
