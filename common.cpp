// Copyright 2019 Michael Johnson

#include "./common.h"

#include <algorithm>
#include <chrono>  // NOLINT(build/c++11)
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

namespace mjohnson {
namespace common {

// This is a specialization for string. It reads entire lines instead of a
// single word.
template <>
std::string RequestInput<std::string>(
    const std::string& prompt,
    const std::function<bool(std::string)>& validator) {
  bool valid = true;
  std::string response;
  do {
    std::cout << prompt;
    std::getline(std::cin, response);

    if (validator) {  // Validator has a bool operator that tells us whether or
                      // not the function is empty
      valid = validator(response);
    } else {
      valid = true;
    }
  } while (!valid);

  return response;
}

bool ParseArgs(int argc, char* argv[], bool* run_unit_tests) {
  if (run_unit_tests == nullptr) {  // Check for null pointer
    throw std::invalid_argument("run_unit_tests");
  }

  *run_unit_tests = false;  // Initialize as false to prevent an uninitialized
                            // variable in main

  if (argc <= 1) {
    // The only argument is the program name
    return true;
  }

  bool bad_arg = false;
  // Skip the first argument, since it's the program path
  for (int i = 1; i < argc; i++) {
    const std::string arg(argv[i]);  // convert to a std::string so that we
                                     // don't have to use strcmp
    if (arg == "-test") {
      *run_unit_tests = true;
    } else {
      bad_arg = true;
      std::cout << "Unexpected argument: " << arg << std::endl;
    }
  }

  return !bad_arg;  // Return false if we have a bad argument; true otherwise
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
bool ValidateContinueResponse(const std::string& response);

bool RequestContinue() {
  return RequestContinue("Would you like to run the program again? [y/N] ");
}

bool RequestContinue(const std::string& prompt) {
  while (true) {
    auto response = RequestInput<std::string>(prompt, ValidateContinueResponse);
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
    throw std::invalid_argument("response");
  }
}

bool ValidateContinueResponse(const std::string& response) {
  if (response.empty()) {
    // Shortcut; we allow empty responses
    return true;
  }

  std::string modified_response = response;  // Copy the response string

  // Lowercase the response to make it easier to compare
  std::transform(modified_response.begin(), modified_response.end(),
                 modified_response.begin(), ::tolower);

  const bool is_valid =
      (modified_response == "y" || modified_response == "yes" ||
       modified_response == "n" || modified_response == "no");
  if (!is_valid) {
    std::cout
        << response
        << " is an invalid response. Available responses are yes, y, no, or n."
        << std::endl
        << std::endl;
  }

  return is_valid;
}

void ClearInputWhitespace() {
  char c = static_cast<char>(std::cin.peek());
  if (std::isspace(c) == 0) {
    // The next character isn't whitespace; leave it alone
    return;
  }
  std::cin.ignore();  // Ignore the whitespace
}

void ClearInvalidInput() {
  std::cin.clear();  // clear the error from cin
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                  '\n');  // Ignore all of the user input currently in the
                          // buffer up to the next new line.
}

std::string GetTimeString(std::chrono::duration<double> duration_s) {
  std::stringstream result_stream;
  result_stream << std::fixed << std::setprecision(0);
  auto hours = std::chrono::duration_cast<std::chrono::hours>(duration_s);
  if (hours.count() > 1) {
    result_stream << hours.count() << "h";
    duration_s -= hours;
  }
  auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration_s);
  if (minutes.count() > 1) {
    result_stream << minutes.count() << "m";
    duration_s -= minutes;
  }
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration_s);
  if (seconds.count() > 1) {
    result_stream << seconds.count() << "s";
    duration_s -= seconds;
  }
  auto milliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration_s);
  if (milliseconds.count() > 1) {
    result_stream << milliseconds.count() << "ms";
    duration_s -= milliseconds;
  }
  auto microseconds =
      std::chrono::duration_cast<std::chrono::microseconds>(duration_s);
  if (microseconds.count() > 1) {
    result_stream << microseconds.count() << "us";
    duration_s -= microseconds;
  }
  auto nanoseconds =
      std::chrono::duration_cast<std::chrono::nanoseconds>(duration_s);
  if (nanoseconds.count() > 1) {
    result_stream << nanoseconds.count() << "ns";
  }

  return result_stream.str();
}

void TrimString(std::string* str) {
  str->erase(str->begin(), std::find_if(str->begin(), str->end(), [](int c) {
               return std::isspace(c) == 0;
             }));  // Erase the string from the beginning until the last
                   // whitespace character
  str->erase(std::find_if(str->rbegin(), str->rend(),
                          [](int c) { return std::isspace(c) == 0; })
                 .base(),
             str->end());  // Erase the string from the last whitespace
                           // character (searching right to left) to the end
}

// Instantiate RequestInput templates for needed types
/*template int32_t RequestInput<int32_t>(
    const std::string& prompt, const std::function<bool(int32_t)>& validator);
template uint32_t RequestInput<uint32_t>(
    const std::string& prompt, const std::function<bool(uint32_t)>& validator);
template uint64_t RequestInput<uint64_t>(
    const std::string& prompt, const std::function<bool(uint64_t)>& validator);
template size_t RequestInput<size_t>(
    const std::string& prompt, const std::function<bool(size_t)>& validator);
template float RequestInput<float>(const std::string& prompt,
                                   const std::function<bool(float)>& validator);
template double RequestInput<double>(
    const std::string& prompt, const std::function<bool(double)>& validator);*/

}  // namespace common
}  // namespace mjohnson
