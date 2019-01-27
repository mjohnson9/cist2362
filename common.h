// Copyright 2019 Michael Johnson

#pragma once

#include <functional>
#include <string>

namespace mjohnson {
namespace common {
// RequestInput returns input from the user. The input is first validated
// against the type specified in the template. Then, if validator is not empty,
// the input is validated against the provided validator function. The validator
// is responsible for displaying an error message if the input is invalid. If
// the input passes both of these validations, it is returned to the caller.
template <typename T>
T RequestInput(const std::string& prompt,
               const std::function<bool(T)>& validator);

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
bool RequestContinue();

// ClearScreen clears all text from the screen. It operates independent of
// platform.
void ClearScreen();

// TrimString trims the whitespace from both ends of the string
void TrimString(std::string* str);
}  // namespace common
}  // namespace mjohnson
