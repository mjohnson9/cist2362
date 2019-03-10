// Copyright 2019 Michael Johnson

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <stdexcept>
#include <string>

#include "../common.h"

namespace mjohnson {
namespace capitals {

// FORWARD DECLARATIONS

// StateCapitals is a wrapper class for the state capitals map. It provides
// convenience functions to help with accessing the map.
class StateCapitals {
 private:
  static const std::map<std::string, std::string> _state_capitals;

  std::default_random_engine _random_generator;
  std::uniform_int_distribution<size_t> _random_distribution;

  static std::map<std::string, std::string> CreateStateCapitals();

 public:
  StateCapitals();

  // GetRandomState returns a random US state.
  std::string GetRandomState();
  // GetCapital returns the capital for a given US state. The state name must be
  // capitalized correctly.
  std::string GetCapital(const std::string& state) const;
};

// ValidateCityResponse validates that a response from the user regarding a
// capital city is usable. It returns true if the response is usuable or outputs
// an error to cout and returns false if not.
bool ValidateCityResponse(std::string response);

// Trim trims whitespace off of both ends of a string.
void Trim(std::string* str);

// ToLower converts a string to lowercase.
void ToLower(std::string* str);

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  StateCapitals capitals;

  mjohnson::common::ClearScreen();

  do {
    const std::string state = capitals.GetRandomState();
    auto response = mjohnson::common::RequestInput<std::string>(
        "What is the capital of " + state + "? ", ValidateCityResponse);

    std::string capital = capitals.GetCapital(state);
    // Prepare the strings to be compared, to allow for human error
    std::string lower_capital = capital;  // Copy capital
    ToLower(&lower_capital);
    Trim(&response);
    ToLower(&response);
    if (lower_capital == response) {
      std::cout << "Correct! The capital of " << state << " is " << capital
                << "." << std::endl
                << std::endl;
    } else {
      std::cout << "Incorrect. The capital of " << state << " is " << capital
                << "." << std::endl
                << std::endl;
    }
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UTILITY FUNCTIONS

StateCapitals::StateCapitals() {
  this->_random_generator = std::default_random_engine(
      std::random_device{}());  // Create a random generator, seeded from the
                                // system random device
  this->_random_distribution = std::uniform_int_distribution<size_t>(
      0, StateCapitals::_state_capitals.size());
}

std::string StateCapitals::GetRandomState() {
  auto i = StateCapitals::_state_capitals.begin();
  std::advance(i, static_cast<int64_t>(
                      this->_random_distribution(this->_random_generator)));
  return i->first;
}

std::string StateCapitals::GetCapital(const std::string& state) const {
  auto it = StateCapitals::_state_capitals.find(state);
  if (it == StateCapitals::_state_capitals.end()) {
    // The given state is not in our map
    throw std::out_of_range("state");
  }
  return it->second;
}

bool ValidateCityResponse(std::string response) {
  Trim(&response);
  if (response.length() == 0) {
    std::cout << "You must provide an answer." << std::endl << std::endl;
    return false;
  }

  return true;
}

// Trim functions courtesy of https://stackoverflow.com/a/217605

// TrimL trims whitespace off of the beginning of a string.
void TrimL(std::string* str) {
  str->erase(str->begin(), std::find_if(str->begin(), str->end(), [](int ch) {
               return std::isspace(ch) == 0;
             }));
}

// TrimR trims whitespace off of the end of a string.
void TrimR(std::string* str) {
  str->erase(std::find_if(str->rbegin(), str->rend(),
                          [](int ch) { return std::isspace(ch) == 0; })
                 .base(),
             str->end());
}

void Trim(std::string* str) {
  TrimL(str);
  TrimR(str);
}

void ToLower(std::string* str) {
  std::transform(str->begin(), str->end(), str->begin(), ::tolower);
}

std::map<std::string, std::string> StateCapitals::CreateStateCapitals() {
  return std::map<std::string, std::string>{{"Alabama", "Montgomery"},
                                            {"Alaska", "Juneau"},
                                            {"Arizona", "Phoenix"},
                                            {"Arkansas", "Little Rock"},
                                            {"California", "Sacramento"},
                                            {"Colorado", "Denver"},
                                            {"Connecticut", "Hartford"},
                                            {"Delaware", "Dover"},
                                            {"Florida", "Tallahassee"},
                                            {"Georgia", "Atlanta"},
                                            {"Hawaii", "Honolulu"},
                                            {"Idaho", "Boise"},
                                            {"Illinois", "Springfield"},
                                            {"Indiana", "Indianapolis"},
                                            {"Iowa", "Des Moines"},
                                            {"Kansas", "Topeka"},
                                            {"Kentucky", "Frankfort"},
                                            {"Louisiana", "Baton Rouge"},
                                            {"Maine", "Augusta"},
                                            {"Maryland", "Annapolis"},
                                            {"Massachusetts", "Boston"},
                                            {"Michigan", "Lansing"},
                                            {"Minnesota", "Saint Paul"},
                                            {"Mississippi", "Jackson"},
                                            {"Missouri", "Jefferson City"},
                                            {"Montana", "Helena"},
                                            {"Nebraska", "Lincoln"},
                                            {"Nevada", "Carson City"},
                                            {"New Hampshire", "Concord"},
                                            {"New Jersey", "Trenton"},
                                            {"New Mexico", "Santa Fe"},
                                            {"New York", "Albany"},
                                            {"North Carolina", "Raleigh"},
                                            {"North Dakota", "Bismarck"},
                                            {"Ohio", "Columbus"},
                                            {"Oklahoma", "Oklahoma City"},
                                            {"Oregon", "Salem"},
                                            {"Pennsylvania", "Harrisburg"},
                                            {"Rhode Island", "Providence"},
                                            {"South Carolina", "Columbia"},
                                            {"South Dakota", "Pierre"},
                                            {"Tennessee", "Nashville"},
                                            {"Texas", "Austin"},
                                            {"Utah", "Salt Lake City"},
                                            {"Vermont", "Montpelier"},
                                            {"Virginia", "Richmond"},
                                            {"Washington", "Olympia"},
                                            {"West Virginia", "Charleston"},
                                            {"Wisconsin", "Madison"},
                                            {"Wyoming", "Cheyenne"}};
}

const std::map<std::string, std::string> StateCapitals::_state_capitals =
    StateCapitals::CreateStateCapitals();

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() { return true; }
}  // namespace capitals
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::capitals::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::capitals::Run();
}

// Grade: 100
