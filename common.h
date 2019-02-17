// Copyright 2019 Michael Johnson

#pragma once

#include <chrono>  // NOLINT(build/c++11)
#include <functional>
#include <iostream>
#include <string>

namespace mjohnson {
namespace common {
// ClearInputWhitespace clears the trailing whitespace after a read from cin
void ClearInputWhitespace();
// ClearInvalidInput clears invalid input from cin and resets any error flags
void ClearInvalidInput();
// RequestInput returns input from the user. The input is first validated
// against the type specified in the template. Then, if validator is not empty,
// the input is validated against the provided validator function. The validator
// is responsible for displaying an error message if the input is invalid. If
// the input passes both of these validations, it is returned to the caller.
template <typename T>
T RequestInput(const std::string& prompt,
               const std::function<bool(T)>& validator) {
  bool valid = true;
  T response;
  do {
    std::cout << prompt;
    std::cin >> response;
    ClearInputWhitespace();

    if (std::cin.fail()) {  // cin.fail returns true when we attempt to extract
                            // a type from the stream, but the data that the
                            // user entered cannot be converted to that type.
      std::cout << "You have given an invalid answer. Please answer the "
                   "question with a valid input."
                << std::endl
                << std::endl;
      valid = false;
      ClearInvalidInput();
      continue;  // Fail fast and attempt another prompt
    }

    if (validator) {  // Validator has a bool operator that tells us whether or
                      // not the function is empty
      valid = validator(response);
    } else {
      valid = true;
    }
  } while (!valid);

  return response;
}

// RequestInput<std::string> returns a line of input from the user. The input
// is validated against the validator provided (if it's not empty). The
// validator is responsible for displaying an error message if the input is
// invalid. If the input passes the validator, it's returned to the caller.
template <>
std::string RequestInput<std::string>(
    const std::string& prompt,
    const std::function<bool(std::string)>& validator);

// ParseArgs parses the arguments passed to a program from the command line. It
// takes pointers to certain flags that it sets as "return" values.
bool ParseArgs(int argc, char* argv[], bool* run_unit_tests);

// RequestContinue prompts the user to ask if they would like to continue the
// program. It continuously re-prompts on invalid input. Once valid input is
// received, it returns the result.
bool RequestContinue(const std::string& prompt);
bool RequestContinue();

// ClearScreen clears all text from the screen. It operates independent of
// platform.
void ClearScreen();

// GetTimeString formats a chrono::duration as a human-readable string
std::string GetTimeString(std::chrono::duration<double> duration_s);

// TrimString trims the whitespace from both ends of the string
void TrimString(std::string* str);
}  // namespace common
}  // namespace mjohnson
