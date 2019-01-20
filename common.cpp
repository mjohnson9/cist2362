// Copyright 2019 Michael Johnson

#include "./common.h"

#include <algorithm>
#include <iostream>
#include <limits>

namespace mjohnson {
namespace common {
// ClearInvalidInput clears invalid input from cin and resets any error flags
void ClearInvalidInput();

// GetTypeName converts a type name into a user friendly type name
template <typename T>
std::string GetTypeName();
template <>
std::string GetTypeName<int>();
template <>
std::string GetTypeName<float>();
template <>
std::string GetTypeName<double>();

template <typename T>
T RequestInput(const std::string& prompt, std::function<bool(T)> validator) {
  bool valid = true;
  T response;
  do {
    std::cout << prompt;
    std::cin >> response;

    if (std::cin.fail()) {  // cin.fail returns true when we attempt to extract
                            // a type from the stream, but the data that the
                            // user entered cannot be converted to that type.
      ClearScreen();
      const std::string type_name = GetTypeName<T>();
      std::cout << "You have given an invalid " << type_name
                << ". Please provide a valid " << type_name << ".";
      valid = false;
      ClearInvalidInput();
      continue;  // Fail fast and attempt another prompt
    }

    ClearScreen();
    if (validator) {  // Validator has a bool operator that tells us whether or
                      // not the function is empty
      valid = validator(response);
    } else {
      valid = true;
    }

    if (!valid) {
      ClearInvalidInput();
    }
  } while (!valid);

  return response;
}

// This is a specialization for string. It reads entire lines instead of a
// single word.
template <>
std::string RequestInput<std::string>(
    const std::string& prompt, std::function<bool(std::string)> validator) {
  bool valid = true;
  std::string response;
  do {
    std::cout << prompt;
    std::getline(std::cin, response);

    ClearScreen();
    if (validator) {  // Validator has a bool operator that tells us whether or
                      // not the function is empty
      valid = validator(response);
    } else {
      valid = true;
    }
  } while (!valid);

  return response;
}

bool ParseArgs(int argc, char* argv[], bool* runUnitTests) {
  if (argc <= 1) {
    // The only argument is the program name
    return true;
  }

  if (runUnitTests == NULL) {  // Check for null pointer
    throw new std::invalid_argument("runUnitTests");
  }

  bool badArg = false;
  // Skip the first argument, since it's the program path
  for (int i = 1; i < argc; i++) {
    const std::string arg(argv[i]);  // convert to a std::string so that we
                                     // don't have to use strcmp
    if (arg == "-test") {
      *runUnitTests = true;
    } else {
      badArg = true;
      std::cout << "Unexpected argument: " << arg << std::endl;
    }
  }

  if (badArg) {
    return false;
  }

  return true;
}

void ClearScreen() {
// It feels nasty forking a process to clear the screen, but it's a better
// alternative to including something like ncurses
#if defined(_WIN32) || defined(_WIN64)
  system("CLS");
#elif defined(unix) || defined(__unix__) || defined(__unix) || \
    (defined(__APPLE__) && defined(__MACH__))
  system("clear");
#else
#error "Operating system not supported"
#endif
}

// ValidateContinueResponse is a validation function for RequestInput. It
// validates that a response to the continue question can be used.
bool ValidateContinueResponse(std::string response);

bool RequestContinue() {
  while (true) {
    auto response = RequestInput<std::string>(
        "Would you like to run the program again? ", ValidateContinueResponse);
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

    // We should never reach this point
    throw new std::invalid_argument("response");
  }
}

bool ValidateContinueResponse(std::string response) {
  if (response.empty()) {
    // Shortcut; we allow empty responses
    return true;
  }

  std::string modifiedResposne = response;  // Copy the response string

  // Lowercase the response to make it easier to compare
  std::transform(modifiedResposne.begin(), modifiedResposne.end(),
                 modifiedResposne.begin(), ::tolower);

  const bool isValid = (modifiedResposne == "y" || modifiedResposne == "yes" ||
                        modifiedResposne == "n" || modifiedResposne == "no");
  if (!isValid) {
    std::cout
        << modifiedResposne
        << " is an invalid response. Available responses are yes, y, no, or n."
        << std::endl
        << std::endl;
  }

  return isValid;
}

void ClearInvalidInput() {
  std::cin.clear();  // Clear the error from cin
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                  '\n');  // Ignore all of the user input currently in the
                          // buffer up to the next new line.
}

template <typename T>
std::string GetTypeName() {
  // There's no specialized function for this type; throw an exception, because
  // we can't identify it
  throw new std::invalid_argument("T");
}
template <>
std::string GetTypeName<int>() {
  return "integer";
}
template <>
std::string GetTypeName<float>() {
  return "number";
}
template <>
std::string GetTypeName<double>() {
  return "number";
}

}  // namespace common
}  // namespace mjohnson
