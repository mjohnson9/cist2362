// Copyright 2019 Michael Johnson

#include <string.h>   // for memmove
#include <cstddef>    // for size_t
#include <cstdint>    // for int32_t, int64_t
#include <iostream>   // for operator<<, endl, basic_ostream
#include <stdexcept>  // for length_error, invalid_argument
#include <string>     // for allocator, operator+, to_string

#include "../common.h"  // for RequestInput, ParseArgs, RequestContinue

namespace mjohnson {
namespace staticqueue {

// FORWARD DECLARATIONS

template <typename T>
class StaticQueue {
 private:
  T* queue_;
  size_t queue_size_;
  size_t queue_capacity_;

 public:
  explicit StaticQueue(size_t capacity);
  ~StaticQueue() { delete[] this->queue_; }

  void Enqueue(T value);
  T Dequeue();

  size_t Size();
  size_t Capacity();
  bool IsFull();
  bool IsEmpty();
};

// MAIN FUNCTIONS
int Run() {
  do {
    auto queue_capacity = mjohnson::common::RequestInput<size_t>(
        "How many items would you like to put in the queue? ", nullptr);
    StaticQueue<int64_t> queue(queue_capacity);

    for (size_t i = 0; i < queue.Capacity(); i++) {
      auto value = mjohnson::common::RequestInput<int64_t>(
          "What value would you like for item #" + std::to_string(i + 1) + "? ",
          nullptr);
      queue.Enqueue(value);
    }

    std::cout << std::endl << "Replaying your queue:" << std::endl;
    for (size_t i = 0; i < queue_capacity; i++) {
      std::cout << "[" << (i + 1) << "]: " << queue.Dequeue() << std::endl;
    }
    std::cout << std::endl;
  } while (mjohnson::common::RequestContinue());
  return 0;
}

// FUNCTION DEFINITIONS

template <typename T>
StaticQueue<T>::StaticQueue(size_t capacity) {
  if (capacity <= 0) {
    throw std::invalid_argument("capacity must be greater than 0");
  }

  this->queue_ = new T[capacity];
  this->queue_size_ = 0;
  this->queue_capacity_ = capacity;
}

template <typename T>
void StaticQueue<T>::Enqueue(T value) {
  if (this->IsFull()) {
    throw std::length_error("queue is full");
  }

  this->queue_[this->queue_size_] = value;
  this->queue_size_ += 1;
}

template <typename T>
T StaticQueue<T>::Dequeue() {
  if (this->IsEmpty()) {
    throw std::length_error("queue is empty");
  }

  T value = this->queue_[0];
  this->queue_size_ -= 1;

  std::memmove(&this->queue_[0], &this->queue_[1],
               sizeof(T) * this->queue_size_);

  return value;
}

template <typename T>
bool StaticQueue<T>::IsFull() {
  return (this->queue_size_ == this->queue_capacity_);
}

template <typename T>
bool StaticQueue<T>::IsEmpty() {
  return (this->queue_size_ == 0);
}

template <typename T>
size_t StaticQueue<T>::Size() {
  return this->queue_size_;
}

template <typename T>
size_t StaticQueue<T>::Capacity() {
  return this->queue_capacity_;
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or
// failure of those unit tests as a boolean.
bool RunUnitTests() {
  bool test_return = true;

  {
    // Test empty stack pop
    StaticQueue<bool> test_queue(1);
    try {
      test_queue.Dequeue();
      std::cerr << "FAIL: Empty queue pop." << std::endl;

      test_return = false;  // NOLINT(clang-analyzer-deadcode.DeadStores)
    } catch (const std::length_error& ex) {
      std::cout << "PASS: Empty queue pop." << std::endl;
    }
  }

  {
    // Test full stack push
    StaticQueue<bool> test_queue(1);
    try {
      test_queue.Enqueue(true);
      test_queue.Enqueue(true);
      std::cerr << "FAIL: Full queue push." << std::endl;

      test_return = false;
    } catch (const std::length_error& ex) {
      std::cout << "PASS: Full queue push." << std::endl;
    }
  }

  {
    // Test retrieving a value
    StaticQueue<int32_t> test_queue(1);
    const int32_t kExpectedValue = 5;

    test_queue.Enqueue(kExpectedValue);

    const int32_t received_value = test_queue.Dequeue();
    if (received_value == kExpectedValue) {
      std::cout << "PASS: Queue enqueue-dequeue operation." << std::endl;
    } else {
      std::cerr << "FAIL: Queue enqueue-dequeue operation: expected "
                << kExpectedValue << ", received " << received_value
                << std::endl;

      test_return = false;
    }
  }

  {
    // Test retrieving multiple values
    const size_t kValueMultiplier = 5;
    const size_t kNumValues = 10;

    StaticQueue<size_t> test_queue(kNumValues);

    for (size_t i = 0; i < kNumValues; i++) {
      test_queue.Enqueue(i * kValueMultiplier);
    }

    bool test_passed = true;
    for (size_t i = 0; i < kNumValues; i--) {
      const size_t expected_value = i * kValueMultiplier;
      const size_t received_value = test_queue.Dequeue();

      if (received_value != expected_value) {
        std::cerr << "FAIL: Queue multiple enqueue-dequeue operation: expected "
                  << expected_value << ", received " << received_value
                  << std::endl;

        test_return = false;
        test_passed = false;
      }
    }

    if (test_passed) {
      std::cout << "PASS: Queue multiple enqueue-dequeue operation."
                << std::endl;
    }
  }

  return test_return;
}
}  // namespace staticqueue
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::staticqueue::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::staticqueue::Run();
}

// Grade: 100
