// Copyright 2019 Michael Johnson
//
// This program

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

#include "../common.h"

namespace monthlytemperatures {
// Forward declarations

// month_names is an array containing the names of the months. It is used for
// convenience and user display.
const char* month_names[] = {"January",   "February", "March",    "April",
                             "May",       "June",     "July",     "August",
                             "September", "October",  "November", "December"};

// TEMP_LOWEST is the index of the second dimension of the array that
// represents the lowest temperature.
const size_t TEMP_LOWEST = 0;
// TEMP_HIGHEST is the index of the second dimension of the array that
// represents the highest temperature.
const size_t TEMP_HIGHEST = 1;

// Returns input from the user. The input is first validated against the type
// specified in the template. Then, if validator is not empty, the input is
// validated against the provided validator function. If the input passes both
// of these validations, it is returned to the caller.
template <typename T>
T RequestInput(const std::string& prompt, std::function<bool(T)> validator);
// Prompts the user to ask if they would like to continue. Re-prompts on
// invalid input. Once valid input is received, it returns the result.
bool RequestContinue();
// GetData requests the monthly temperature data from the user.
void GetData(float temperatures[12][2]);
// AverageHigh calculates and returns the average high temperature based on
// the highest temperatures for each month.
float AverageHigh(float temperatures[12][2]);
// AverageLow calculates and returns the average low temperature based on the
// lowest temperatures for each month.
float AverageLow(float temperatures[12][2]);
// IndexHighTemp finds the index of the month with the highest temperature and
// returns the index of that month.
size_t IndexHighTemp(float temperatures[12][2]);
// IndexHighTemp finds the index of the month with the lowest temperature and
// returns the index of that month.
size_t IndexLowTemp(float temperatures[12][2]);
// ValidateFarenheitTemperature is a function to be passed to RequestInput as
// a validator. It validates that the given temperature is a valid Farenheit
// temperature.
bool ValidateFarenheitTemperature(float temperature);
// HandleInvalidInput tells the user that their input was invalid and then
// prepares cin for another attempt.
void HandleInvalidInput();

// Function definitions

void Run() {
  do {
    float temperatures[12][2];
    GetData(temperatures);

    std::cout << "The average high temperature for the year was " << std::fixed
              << std::setprecision(1) << AverageHigh(temperatures) << "."
              << std::endl;
    std::cout.unsetf(std::ios_base::floatfield);

    std::cout << "The average low temperature for the year was " << std::fixed
              << std::setprecision(1) << AverageLow(temperatures) << "."
              << std::endl;
    std::cout.unsetf(std::ios_base::floatfield);

    const size_t highest_temp_index = IndexHighTemp(temperatures);
    std::cout << "The highest temperature for the year was " << std::fixed
              << std::setprecision(1)
              << temperatures[highest_temp_index][TEMP_HIGHEST] << "."
              << std::endl;
    std::cout.unsetf(std::ios_base::floatfield);

    const size_t lowest_temp_index = IndexLowTemp(temperatures);
    std::cout << "The lowest temperature for the year was " << std::fixed
              << std::setprecision(1)
              << temperatures[lowest_temp_index][TEMP_LOWEST] << "."
              << std::endl;
    std::cout.unsetf(std::ios_base::floatfield);

    std::cout << std::endl;  // Extra line to separate the continue question
  } while (RequestContinue());
}

void GetData(float temperatures[12][2]) {
  for (size_t month = 0; month < 12; month++) {
    const std::string month_name = month_names[month];

    bool valid = true;
    do {
      const auto lowest_temp = RequestInput<float>(
          "In Farenheit, what was the lowest temperature for the month of " +
              month_name + "? ",
          ValidateFarenheitTemperature);

      const auto highest_temp = RequestInput<float>(
          "In Farenheit, what was the highest temperature for the month of " +
              month_name + "? ",
          ValidateFarenheitTemperature);

      if (lowest_temp > highest_temp) {
        std::cout << "The lowest temperature cannot be higher than the"
                  << " highest temperature." << std::endl
                  << std::endl;
        valid = false;
      } else {
        temperatures[month][TEMP_LOWEST] = lowest_temp;
        temperatures[month][TEMP_HIGHEST] = highest_temp;
        valid = true;
      }
    } while (!valid);
  }

  std::cout << std::endl << std::endl;
}

float AverageHigh(float temperatures[12][2]) {
  float sum = 0;
  for (size_t month = 0; month < 12; month++) {
    sum += temperatures[month][TEMP_HIGHEST];
  }
  return sum / 12.0f;  // Get the average by dividing by the number of months
}

float AverageLow(float temperatures[12][2]) {
  float sum = 0;
  for (size_t month = 0; month < 12; month++) {
    sum += temperatures[month][TEMP_LOWEST];
  }
  return sum / 12.0f;  // Get the average by dividing by the number of months
}

size_t IndexHighTemp(float temperatures[12][2]) {
  size_t highest_temp_index = 0;
  float highest_temp = temperatures[highest_temp_index][TEMP_HIGHEST];

  // In the upcoming loop, we skip index 0 because it's already read into the
  // highest temperature variables
  for (size_t month = 1; month < 12; month++) {
    const float month_highest = temperatures[month][TEMP_HIGHEST];
    if (month_highest > highest_temp) {
      highest_temp_index = month;
      highest_temp = month_highest;
    }
  }

  return highest_temp_index;
}

size_t IndexLowTemp(float temperatures[12][2]) {
  size_t lowest_temp_index = 0;
  float lowest_temp = temperatures[lowest_temp_index][TEMP_LOWEST];

  // In the upcoming loop, we skip index 0 because it's already read into the
  // highest temperature variables
  for (size_t month = 1; month < 12; month++) {
    const float month_lowest = temperatures[month][TEMP_LOWEST];
    if (month_lowest < lowest_temp) {
      lowest_temp_index = month;
      lowest_temp = month_lowest;
    }
  }

  return lowest_temp_index;
}

template <typename T>
T RequestInput(const std::string& prompt, std::function<bool(T)> validator) {
  bool valid = true;
  T response;
  do {
    std::cout << prompt;
    std::cin >> response;

    if (std::cin.fail()) {  // cin.fail returns true when we attempt to
                            // extract a type from the stream, but the data
                            // that the user entered cannot be converted to
                            // that type.
      valid = false;
      HandleInvalidInput();
      continue;  // Fail fast and attempt another prompt
    }

    if (validator) {  // Validator has a bool operator that tells us whether
                      // or not the function is empty
      valid = validator(response);
    } else {
      valid = true;
    }

    if (!valid) {
      HandleInvalidInput();
    }
  } while (!valid);

  return response;
}

bool RequestContinue() {
  while (true) {
    std::cout << "Would you like to run the program again? ";

    std::string response;
    {  // Put this in its own scope to reduce scope pollution
      char next_char;
      do {
        next_char = static_cast<char>(std::cin.peek());
        if (next_char == '\r' || next_char == '\n') {
          std::cin.get(next_char);  // Remove the newline from the buffer
        }
      } while (next_char == '\r' || next_char == '\n');  // Continue as long
                                                         // as we encounter
                                                         // newlines
    }
    std::getline(std::cin, response);
    if (response.empty()) {
      return false;  // The default option is to not continue
    }

    // Lowercase the response to make it easier to compare
    std::transform(response.begin(), response.end(), response.begin(),
                   ::tolower);
    if (response == "y" || response == "yes") {
      return true;
    }
    if (response == "n" || response == "no") {
      return false;
    }
    std::cout << response << " is an invalid response. Available responses"
              << " are yes, y, no, or n." << std::endl
              << std::endl;
  }
}

bool ValidateFarenheitTemperature(float temperature) {
  return temperature > -459.67f;  // -459.67F is absolute zero; a Farenheit
                                  // temperature cannot be lower than that.
}

void HandleInvalidInput() {
  std::cout << "The given input was invalid." << std::endl << std::endl;
  std::cin.clear();               // Clear the error from cin
  std::cin.ignore(0xFFFF, '\n');  // Ignore (hopefully) all of the user input
                                  // currently in the buffer up to the next
                                  // new line.
}
}  // namespace monthlytemperatures

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    std::cout << "This program has no unit tests." << std::endl;
    return 0;
  }

  monthlytemperatures::Run();
  return 0;
}
