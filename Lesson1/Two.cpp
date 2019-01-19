// Copyright 2019 Michael Johnson
//
// This program

#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

namespace loancalculator {
  // Forward declarations

  // Returns input from the user. The input is first validated against the type
  // specified in the template. Then, if validator is not empty, the input is
  // validated against the provided validator function. If the input passes both
  // of these validations, it is returned to the caller.
  template<typename T> T RequestInput(const std::string& prompt,
                           std::function<bool(T)> validator);
  // CalculateInterest calculates the month's interest rate. It assumes that
  // interest_rate is a yearly interest rate.
  double CalculateInterest(double loan, double interest_rate);
  // CalculateNumberOfMonths calculates and returns the number of months it will
  // take to pay off a loan with the given parameters. It returns -1 when a loan
  // will never be paid off.
  int CalculateNumberOfMonths(double loan, double interest_rate,
                              double payment);
  // Validator function for use in RequestInput. Validates that the given double
  // is greater than 0.
  bool ValidatePositiveFloat(double to_check);
  // Prompts the user to ask if they would like to continue. Re-prompts on
  // invalid input. Once valid input is received, it returns the result.
  bool RequestContinue();
  // HandleInvalidInput tells the user that their input was invalid and then
  // prepares cin for another attempt.
  void HandleInvalidInput();

  // Function definitions

  void Run() {
    do {
      // Ask the user for the requisite information
      auto loan = RequestInput<double>("What is the loan amount? ",
                                       ValidatePositiveFloat);
      auto interest_rate = RequestInput<double>(
          "What is the yearly interest rate? ",
          ValidatePositiveFloat);
      auto payment = RequestInput<double>("What is the monthly payment? ",
                                       ValidatePositiveFloat);

      interest_rate /= 100.0f;  // Divide interest rate by 100 to get a
                                // percentage that we can use

      const double first_months_interest =
          CalculateInterest(loan, interest_rate);
      if (payment <= first_months_interest) {
        std::cout << std::endl
                  << "With the given parameters, the loan will "
                  << "never be paid off. The first month's interest is $"
                  << std::fixed << std::setprecision(2) << first_months_interest
                  << " while the payment is only $" << payment << "."
                  << std::defaultfloat << std::endl
                  << std::endl;
        continue;  // Go to the next iteration of the loop
      }

      int num_months = CalculateNumberOfMonths(loan, interest_rate, payment);

      std::cout << std::endl << "It will take " << num_months << " "
                << (num_months != 1 ? "months" : "month") << " to pay off the "
                << "loan." << std::endl << std::endl;
    } while (RequestContinue());
  }

  int CalculateNumberOfMonths(double loan, double interest_rate,
                              double payment) {
    const double monthly_interest_rate = interest_rate / 12.0f;
    // I love Wolfram|Alpha
    uint64_t num_months = ceil(
        -(std::log(1.0f - ((loan * monthly_interest_rate) / payment)) /
          std::log(1.0f + monthly_interest_rate)));
    return num_months;

    /*
    uint64_t num_months = 0;
    while (true) {  // Iterate once for each month of payment
      const double months_interest = CalculateInterest(loan, interest_rate);
      if (months_interest > payment) {
        // The interest is greater than the loan amount. As such, the loan will
        // never be paid off.
        return -1;
      }

      // The amount left after interest is deducted
      const double remaining_payment = payment - months_interest;
      loan -= remaining_payment;

      num_months++;

      if (loan <= 0) {
        // The loan has been paid off. Break out of the loop so that we can
        // return the number of months it took.
        break;
      }
    }

    return num_months;
    */
  }

  double CalculateInterest(double loan, double interest_rate) {
    double monthly_interest_rate = interest_rate / 12.0f;
    double months_interest = loan * monthly_interest_rate;
    return months_interest;
  }

  template<typename T> T RequestInput(const std::string& prompt,
                           std::function<bool(T)> validator) {
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
      { // Put this in its own scope to reduce scope pollution
        char next_char;
        do {
          next_char = std::cin.peek();
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
        << " are yes, y, no, or n." << std::endl << std::endl;
    }
  }

  bool ValidatePositiveFloat(double to_check) {
    return to_check > 0.0f;
  }

  void HandleInvalidInput() {
    std::cout << "The given input was invalid." << std::endl << std::endl;
    std::cin.clear();  // Clear the error from cin
    std::cin.ignore(0xFFFF, '\n');  // Ignore (hopefully) all of the user input
                                    // currently in the buffer up to the next
                                    // new line.
  }
}  // namespace loancalculator

int main() {
  loancalculator::Run();
  return 0;
}
