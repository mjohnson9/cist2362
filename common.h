// Copyright 2019 Michael Johnson

#ifndef COMMON_H_
#define COMMON_H_

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
T RequestInput(const std::string& prompt, std::function<bool(T)> validator);

// RequestInput<std::string> returns a line of input from the user. The input
// is validated against the validator provided (if it's not empty). The
// validator is responsible for displaying an error message if the input is
// invalid. If the input passes the validator, it's returned to the caller.
template <>
std::string RequestInput<std::string>(
    const std::string& prompt, std::function<bool(std::string)> validator);

// RequestContinue prompts the user to ask if they would like to continue the
// program. It continuously re-prompts on invalid input. Once valid input is
// received, it returns the result.
bool RequestContinue();
}  // namespace common
}  // namespace mjohnson

#endif  // COMMON_H_
