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

// Validator function for use in RequestInput. Validates that the number of
// copies is greater than 0.
bool ValidateCopies(int copies);
// Validator function for use in RequestInput. Validates that the net price is
// greater than 0.
bool ValidateNetPrice(float net_price);
// Calculates the royalties under a given royalty option using the copies and
// net_price given.
float CalculateRoyalties(RoyaltyOption option, int copies, float net_price);
// Prints the royalties of each option and provides the best option based on
// the number of copies and net_price given.
void PrintRoyalties(int copies, float net_price);
// RoyaltyOptionToString gives a proper name for a royalty option.
std::string RoyaltyOptionToString(size_t option_index, bool lowercase);

// Provides the primary run loop for the program.
void Run() {
  do {
    const int copies = mjohnson::common::RequestInput<int>(
        "How many copies do you expect to sell? ", ValidateCopies);
    const auto net_price = mjohnson::common::RequestInput<float>(
        "What is the net price of the title? ", ValidateNetPrice);
    PrintRoyalties(copies, net_price);
  } while (mjohnson::common::RequestContinue());
}

float CalculateRoyalties(RoyaltyOption option, int copies, float net_price) {
  switch (option) {
    case FIRST:
      return 5000.0f + 20000.0f;
    case SECOND:
      return static_cast<float>(copies) * (net_price * 0.125f);
    case THIRD:
      return (static_cast<float>(std::min(4000, copies)) *
              (net_price * 0.10f)) +
             (static_cast<float>(std::max(0, copies - 4000)) *
              (net_price * 0.14f));
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

bool ValidateCopies(int copies) { return copies > 0; }

bool ValidateNetPrice(float net_price) { return net_price > 0; }

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
