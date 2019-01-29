// Copyright 2019 Michael Johnson
//
// This small program calculates royalties for an author based on a publisher's
// provided rules.

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

#include "../common.h"

namespace authorcontract {
enum RoyaltyOption { FIRST, SECOND, THIRD };

// Forward declarations

// Returns input from the user. The input is first validated against the type
// specified in the template. Then, if validator is not empty, the input is
// validated against the provided validator function. If the input passes both
// of these validations, it is returned to the caller.
template <typename T>
T RequestInput(const std::string& prompt, std::function<bool(T)> validator);
// Validator function for use in RequestInput. Validates that the number of
// copies is greater than 0.
bool ValidateCopies(int copies);
// Validator function for use in RequestInput. Validates that the net price is
// greater than 0.
bool ValidateNetPrice(float net_price);
// Prompts the user to ask if they would like to continue. Re-prompts on
// invalid input. Once valid input is received, it returns the result.
bool RequestContinue();
// Calculates the royalties under a given royalty option using the copies and
// net_price given.
float CalculateRoyalties(RoyaltyOption option, int copies, float net_price);
// Prints the royalties of each option and provides the best option based on
// the number of copies and net_price given.
void PrintRoyalties(int copies, float net_price);
// HandleInvalidInput tells the user that their input was invalid and then
// prepares cin for another attempt.
void HandleInvalidInput();
// RoyaltyOptionToString gives a proper name for a royalty option.
std::string RoyaltyOptionToString(size_t option_index, bool lowercase);

// Provides the primary run loop for the program.
void Run() {
  do {
    const int copies = RequestInput<int>(
        "How many copies do you expect to sell? ", ValidateCopies);
    const auto net_price = RequestInput<float>(
        "What is the net price of the title? ", ValidateNetPrice);
    PrintRoyalties(copies, net_price);
  } while (RequestContinue());
}

float CalculateRoyalties(RoyaltyOption option, int copies, float net_price) {
  switch (option) {
    case FIRST:
      return 5000.0f + 20000.0f;
    case SECOND:
      return copies * (net_price * 0.125f);
    case THIRD:
      return (std::min(4000, copies) * (net_price * 0.10f)) +
             (std::max(0, copies - 4000) * (net_price * 0.14f));
    default:
      // If we received a royalty option that we don't know about, throw an
      // exception for the caller to handle or the program to terminate.
      throw std::invalid_argument("option");
  }
}

void PrintRoyalties(int copies, float net_price) {
  const int num_royalties = 3;
  float royalties[num_royalties] = {
      CalculateRoyalties(FIRST, copies, net_price),
      CalculateRoyalties(SECOND, copies, net_price),
      CalculateRoyalties(THIRD, copies, net_price)};

  // Print the royalty outcomes
  std::cout << "|---------------------------" << std::endl
            << "| Royalty results:" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  for (size_t i = 0; i < num_royalties; i++) {
    std::cout << "| " << RoyaltyOptionToString(i, false) << ": $"
              << royalties[i] << std::endl;
  }
  std::cout.unsetf(std::ios_base::floatfield);

  // Find the largest royalty's index
  size_t largest_royalty = 0;
  bool largest_royalties[num_royalties] = {true, false, false};

  for (size_t i = 1; i < num_royalties; i++) {
    if (royalties[i] > royalties[largest_royalty]) {
      largest_royalty = i;
      for (bool& largest_royaltie : largest_royalties) {
        largest_royaltie = false;
      }
      largest_royalties[i] = true;
    } else if (royalties[i] == royalties[largest_royalty]) {
      largest_royalties[i] = true;
    }
  }

  int num_royalties_equal = 0;
  for (bool largest_royaltie : largest_royalties) {
    if (largest_royaltie) {
      num_royalties_equal++;
    }
  }

  switch (num_royalties_equal) {
    case 1: {  // We must enclose these in blocks to limit their scope.
               // Otherwise, the switch statement can't jump past the variable
               // declaration.
      std::string royalty_name = RoyaltyOptionToString(largest_royalty, true);
      std::cout << "| Your best royalty option would be the " << royalty_name
                << " one. It will provide $" << std::fixed
                << std::setprecision(2) << royalties[largest_royalty] << "."
                << std::endl;
      std::cout.unsetf(std::ios_base::floatfield);
    } break;
    case 2: {
      std::string royalty_name1, royalty_name2;
      for (size_t i = 0; i < num_royalties; i++) {
        if (!largest_royalties[i]) {
          continue;  // Ignore this iteration of the loop if this royalty
                     // isn't in the list of largest royalties.
        }
        if (royalty_name1.empty()) {
          royalty_name1 = RoyaltyOptionToString(i, true);
        } else {
          royalty_name2 = RoyaltyOptionToString(i, true);
        }
      }
      std::cout << "| Both the " << royalty_name1 << " and " << royalty_name2
                << " royalty would be the best option. They both provide $"
                << std::fixed << std::setprecision(2)
                << royalties[largest_royalty] << "." << std::endl;
      std::cout.unsetf(std::ios_base::floatfield);
    } break;
    case 3: {
      std::cout << "| None of the royalty options would be better than the "
                << "other. They all provide $" << std::fixed
                << std::setprecision(2) << royalties[largest_royalty] << "."
                << std::endl;
      std::cout.unsetf(std::ios_base::floatfield);
    } break;
    default:
      // It should be impossible to reach this point. Thus, if we do reach
      // this point, we throw an error.
      throw std::out_of_range("num_royalties_equal");
  }

  std::cout << "|---------------------------" << std::endl << std::endl;
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

    std::cout << response
              << " is an invalid response. Available responses are yes, y, "
                 "no, or n."
              << std::endl
              << std::endl;
  }
}

bool ValidateCopies(int copies) { return copies > 0; }

bool ValidateNetPrice(float net_price) { return net_price > 0; }

void HandleInvalidInput() {
  std::cout << "The given input was invalid." << std::endl << std::endl;
  std::cin.clear();               // Clear the error from cin
  std::cin.ignore(0xFFFF, '\n');  // Ignore (hopefully) all of the user input
                                  // currently in the buffer up to the next
                                  // new line.
}

std::string RoyaltyOptionToString(size_t option_index, bool lowercase) {
  switch (option_index) {
    case 0:
      return lowercase ? "first" : "First";
    case 1:
      return lowercase ? "second" : "Second";
    case 2:
      return lowercase ? "third" : "Third";
    default:
      return lowercase ? "unknown" : "Unknown";
  }
}
}  // namespace authorcontract

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    std::cout << "This program has no unit tests." << std::endl;
    return 0;
  }

  authorcontract::Run();
  return 0;
}
