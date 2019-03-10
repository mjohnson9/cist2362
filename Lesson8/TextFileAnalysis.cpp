// Copyright 2019 Michael Johnson

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "../common.h"

namespace mjohnson {
namespace textfileanalysis {

// FORWARD DECLARATIONS

// ReadWordsFromFile reads all of the words in a file and places them in a
// vector, in the order that they were read.
std::set<std::string> ReadWordsFromFile(const std::string& file_name);

// ValidateFileName validates that a user's input is a valid, existant, readable
// file.
bool ValidateFileName(const std::string& file_name);

void PrintUnion(const std::set<std::string>& first_set,
                const std::set<std::string>& second_set);

void PrintDifference(const std::set<std::string>& first_set,
                     const std::string& first_set_name,
                     const std::set<std::string>& second_set,
                     const std::string& second_set_name);

void PrintSymmetricDifference(const std::set<std::string>& first_set,
                              const std::set<std::string>& second_set);

void PrintWords(const std::vector<std::string>& words);

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  mjohnson::common::ClearScreen();

  do {
    const auto first_file = mjohnson::common::RequestInput<std::string>(
        "What is the name of the first file to read? ", ValidateFileName);
    std::set<std::string> first_set = ReadWordsFromFile(first_file);

    const auto second_file = mjohnson::common::RequestInput<std::string>(
        "What is the name of the first file to read? ", ValidateFileName);
    std::set<std::string> second_set = ReadWordsFromFile(second_file);

    if (first_set.empty() && second_set.empty()) {
      std::cout << "The given files were empty." << std::endl << std::endl;
      continue;
    } else if (first_set.empty()) {
      std::cout << "The first file given was empty." << std::endl << std::endl;
      continue;
    } else if (second_set.empty()) {
      std::cout << "The second file given was empty." << std::endl << std::endl;
      continue;
    }

    PrintUnion(first_set, second_set);
    PrintDifference(first_set, "first file", second_set, "second file");
    PrintDifference(second_set, "second file", first_set, "first file");
    PrintSymmetricDifference(first_set, second_set);
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UTILITY FUNCTIONS

std::set<std::string> ReadWordsFromFile(const std::string& file_name) {
  std::ifstream file(file_name);
  if (!file.good()) {
    throw std::system_error(errno, std::generic_category(), "file_name");
  }

  std::set<std::string> return_set;

  while (file.good()) {
    std::string word;
    file >> word;
    return_set.insert(word);
  }

  return return_set;
}

void PrintUnion(const std::set<std::string>& first_set,
                const std::set<std::string>& second_set) {
  std::vector<std::string> word_union(first_set.size() + second_set.size());
  auto output_iterator =
      std::set_union(first_set.begin(), first_set.end(), second_set.begin(),
                     second_set.end(), word_union.begin());
  // Resize the vector to fit the results
  word_union.resize(output_iterator - word_union.begin());

  std::cout << "Words in both files:" << std::endl;
  PrintWords(word_union);
}

void PrintDifference(const std::set<std::string>& first_set,
                     const std::string& first_set_name,
                     const std::set<std::string>& second_set,
                     const std::string& second_set_name) {
  size_t difference_size = first_set.size() > second_set.size()
                               ? first_set.size()
                               : second_set.size();

  std::vector<std::string> difference(difference_size);
  auto output_iterator =
      set_difference(first_set.begin(), first_set.end(), second_set.begin(),
                     second_set.end(), difference.begin());
  difference.resize(output_iterator - difference.begin());

  std::cout << "Words in the " << first_set_name << ", but not the "
            << second_set_name << ":" << std::endl;
  PrintWords(difference);
}

void PrintSymmetricDifference(const std::set<std::string>& first_set,
                              const std::set<std::string>& second_set) {
  size_t difference_size = first_set.size() > second_set.size()
                               ? first_set.size()
                               : second_set.size();

  std::vector<std::string> difference(difference_size);
  auto output_iterator = set_symmetric_difference(
      first_set.begin(), first_set.end(), second_set.begin(), second_set.end(),
      difference.begin());
  difference.resize(output_iterator - difference.begin());

  std::cout << "Words in one file or the other, but not both:" << std::endl;
  PrintWords(difference);
}

void PrintWords(const std::vector<std::string>& words) {
  int line_length = 0;
  for (auto it = words.begin(); it != words.end(); it++) {
    const auto word = *it;
    if (line_length != 0) {
      std::cout << ", ";
      line_length += 2;
    }
    std::cout << word;
    line_length += word.length();

    if (line_length >= 80) {
      std::cout << std::endl;
    }
  }

  if (line_length != 0) {
    std::cout << std::endl;
  }
}

bool ValidateFileName(const std::string& file_name) {
  if (file_name.length() == 0) {
    std::cout << "You must provide a file name." << std::endl << std::endl;
    return false;
  }

  std::ifstream testOpen(file_name);
  if (!testOpen.good()) {
    std::cout << "Unable to open " << file_name
              << " for reading: " << strerror(errno) << std::endl
              << std::endl;
    return false;
  }

  testOpen.close();
  return true;
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() { return true; }
}  // namespace textfileanalysis
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::textfileanalysis::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::textfileanalysis::Run();
}

// Grade: 100
