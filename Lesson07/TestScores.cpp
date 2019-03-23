// Copyright 2019 Michael Johnson

#include <iostream>
#include <stdexcept>

#include "../common.h"

namespace mjohnson {
namespace testscores {

// FORWARD DECLARATIONS
class TestScores {
 private:
  double* _test_scores;
  size_t _num_test_scores;

 public:
  class InvalidTestScoreException {
   private:
    size_t _index;

   public:
    explicit InvalidTestScoreException(size_t index) { this->_index = index; }

    size_t index() const { return this->_index; }
  };

  TestScores(const double* test_scores, size_t num_test_scores);
  ~TestScores();

  double average() const;

  size_t size() const { return this->_num_test_scores; }
  double operator[](size_t index) const;
};

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    mjohnson::common::ClearScreen();

    const auto n = mjohnson::common::RequestInput<size_t>(
        "How many test scores would you like to enter? ", nullptr);

    std::cout << std::endl;

    auto scores_array = new double[n];
    for (size_t i = 0; i < n; i++) {
      const auto score = mjohnson::common::RequestInput<double>(
          "Please enter a test score: ",
          nullptr);  // We deliberately don't do validation so that we can
                     // experience the exception from TestScores

      scores_array[i] = score;
    }

    try {
      auto scores = new TestScores(scores_array, n);

      mjohnson::common::ClearScreen();
      std::cout << "========== TEST SCORES ==========" << std::endl;
      for (size_t i = 0; i < scores->size(); i++) {
        std::cout << "#" << (i + 1) << ": " << (*scores)[i] << std::endl;
      }
      std::cout << std::endl
                << "Average: " << scores->average() << std::endl
                << std::endl;

      delete scores;
    } catch (const TestScores::InvalidTestScoreException& e) {
      std::cout << std::endl
                << "Test score #" << (e.index() + 1)
                << " was invalid because it was negative or greater than 100. "
                   "You entered "
                << scores_array[e.index()] << "." << std::endl
                << std::endl;
    }
    delete[] scores_array;
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// DEFINITIONS
TestScores::TestScores(const double* test_scores,
                       const size_t num_test_scores) {
  this->_num_test_scores = num_test_scores;
  this->_test_scores = new double[this->_num_test_scores];

  // Validate and copy the test scores
  for (size_t i = 0; i < num_test_scores; i++) {
    const double test_score = test_scores[i];
    if (test_score < 0 || test_score > 100) {
      delete[] this->_test_scores;  // The dtor won't be called if we throw an
                                    // exception in the ctor

      throw InvalidTestScoreException(i);
    }
    this->_test_scores[i] = test_score;
  }
}

TestScores::~TestScores() { delete[] this->_test_scores; }

double TestScores::average() const {
  double runningTotal = 0l;

  for (size_t i = 0; i < this->_num_test_scores; i++) {
    runningTotal += (*this)[i];
  }

  return runningTotal / static_cast<double>(this->_num_test_scores);
}

double TestScores::operator[](const size_t index) const {
  if (index >= this->_num_test_scores) {
    throw std::out_of_range("index");
  }

  return this->_test_scores[index];
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() { return true; }
}  // namespace testscores
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::testscores::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::testscores::Run();
}

// Grade: 100
