// Copyright 2019 Michael Johnson

#include <iomanip>
#include <iostream>

#include "../common.h"

namespace mjohnson {
namespace customeraccounts {
// STRUCT DEFINITIONS

// BudgetDifferences represents the difference between two budgets
struct BudgetDifferences {
  double housing;
  double utilities;
  double household_expenses;
  double transportation;
  double food;
  double medical;
  double insurance;
  double entertainment;
  double clothing;
  double miscellaneous;

  double total_difference;
};

struct MonthlySpending {
  double housing;
  double utilities;
  double household_expenses;
  double transportation;
  double food;
  double medical;
  double insurance;
  double entertainment;
  double clothing;
  double miscellaneous;
};

// MonthlyBudget represents a monthly budget or monthly spending
struct MonthlyBudget {
  double housing;
  double utilities;
  double household_expenses;
  double transportation;
  double food;
  double medical;
  double insurance;
  double entertainment;
  double clothing;
  double miscellaneous;
};

// FUNCTION FORWARD DECLARATIONS

// GetMonthlyBudget returns the target monthly budget
MonthlyBudget GetMonthlyBudget();
// PromptMonthlySpending asks the user for their monthly spending
MonthlySpending PromptMonthlySpending();
// CompareBudgetToSpending compares a budget to spending and returns the result
// as a differences struct
BudgetDifferences CompareBudgetToSpending(const MonthlyBudget& budget,
                                          const MonthlySpending& spending);
// DisplayMonthlyBudget prints a monthly budget to std::cout
void DisplayMonthlyBudget(const MonthlyBudget& budget);
// DisplayBudgetDifferences prints a report of differences between spending and
// budget to std::cout
void DisplayBudgetDifferences(const BudgetDifferences& differences);
// ValidateSpending validates a user's input for spending is acceptable
bool ValidateSpending(double spending);

// MAIN FUNCTIONS

int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    mjohnson::common::ClearScreen();

    MonthlyBudget budget = GetMonthlyBudget();
    DisplayMonthlyBudget(budget);
    std::cout << std::endl;

    MonthlySpending spending = PromptMonthlySpending();

    BudgetDifferences differences = CompareBudgetToSpending(budget, spending);

    mjohnson::common::ClearScreen();
    DisplayBudgetDifferences(differences);
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UNIT TESTING

bool AssertDifference(double result, double expected, const std::string& name) {
  if (result != expected) {
    std::cout << "FAILED: " << name << ": expected " << expected << ", got "
              << result << std::endl;
  }

  return true;
}

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() {
  MonthlyBudget budget;
  budget.housing = 1l;
  budget.utilities = 1l;
  budget.household_expenses = 1l;
  budget.transportation = 1l;
  budget.food = 1l;
  budget.medical = 1l;
  budget.insurance = 1l;
  budget.entertainment = 1l;
  budget.clothing = 1l;
  budget.miscellaneous = 1l;

  MonthlySpending spending;
  spending.housing = 2l;
  spending.utilities = 2l;
  spending.household_expenses = 2l;
  spending.transportation = 2l;
  spending.food = 2l;
  spending.medical = 2l;
  spending.insurance = 2l;
  spending.entertainment = 2l;
  spending.clothing = 2l;
  spending.miscellaneous = 2l;

  bool success = true;

  BudgetDifferences differences = CompareBudgetToSpending(budget, spending);

  success =
      (AssertDifference(differences.housing, -1, "BudgetDifferences.housing") &&
       success);
  success = (AssertDifference(differences.utilities, -1,
                              "BudgetDifferences.utilities") &&
             success);
  success = (AssertDifference(differences.household_expenses, -1,
                              "BudgetDifferences.household_expenses") &&
             success);
  success = (AssertDifference(differences.transportation, -1,
                              "BudgetDifferences.transportation") &&
             success);
  success = (AssertDifference(differences.food, -1, "BudgetDifferences.food") &&
             success);
  success =
      (AssertDifference(differences.medical, -1, "BudgetDifferences.medical") &&
       success);
  success = (AssertDifference(differences.insurance, -1,
                              "BudgetDifferences.insurance") &&
             success);
  success = (AssertDifference(differences.entertainment, -1,
                              "BudgetDifferences.entertainment") &&
             success);
  success = (AssertDifference(differences.clothing, -1,
                              "BudgetDifferences.clothing") &&
             success);
  success = (AssertDifference(differences.miscellaneous, -1,
                              "BudgetDifferences.miscellaneous") &&
             success);
  success = (AssertDifference(differences.total_difference, -10,
                              "BudgetDifferences.total_difference") &&
             success);

  return success;
}

// UTILITY FUNCTION IMPLEMENTATIONS

MonthlyBudget GetMonthlyBudget() {
  MonthlyBudget budget;
  budget.housing = 500l;
  budget.utilities = 150l;
  budget.household_expenses = 65l;
  budget.transportation = 50l;
  budget.food = 250l;
  budget.medical = 30l;
  budget.insurance = 100l;
  budget.entertainment = 150l;
  budget.clothing = 75l;
  budget.miscellaneous = 50l;
  return budget;
}

MonthlySpending PromptMonthlySpending() {
  MonthlySpending spending;
  spending.housing = mjohnson::common::RequestInput<double>(
      "What did you spend on housing for the month? ", ValidateSpending);
  spending.utilities = mjohnson::common::RequestInput<double>(
      "What did you spend on utilities for the month? ", ValidateSpending);
  spending.household_expenses = mjohnson::common::RequestInput<double>(
      "What did you spend on household expenses for the month? ",
      ValidateSpending);
  spending.transportation = mjohnson::common::RequestInput<double>(
      "What did you spend on transportation for the month? ", ValidateSpending);
  spending.food = mjohnson::common::RequestInput<double>(
      "What did you spend on food for the month? ", ValidateSpending);
  spending.medical = mjohnson::common::RequestInput<double>(
      "What did you spend on medical for the month? ", ValidateSpending);
  spending.insurance = mjohnson::common::RequestInput<double>(
      "What did you spend on insurance for the month? ", ValidateSpending);
  spending.entertainment = mjohnson::common::RequestInput<double>(
      "What did you spend on entertainment for the month? ", ValidateSpending);
  spending.clothing = mjohnson::common::RequestInput<double>(
      "What did you spend on clothing for the month? ", ValidateSpending);
  spending.miscellaneous = mjohnson::common::RequestInput<double>(
      "What did you spend on miscellaneous for the month? ", ValidateSpending);

  return spending;
}

void DisplayMonthlyBudget(const MonthlyBudget& budget) {
  std::cout << std::fixed << std::setprecision(2) << "Budget:" << std::endl
            << "| Housing: " << budget.housing << std::endl
            << "| Utilities: " << budget.utilities << std::endl
            << "| Household Expenses: " << budget.household_expenses
            << std::endl
            << "| Transportation: " << budget.transportation << std::endl
            << "| Food: " << budget.food << std::endl
            << "| Medical: " << budget.medical << std::endl
            << "| Insurance: " << budget.insurance << std::endl
            << "| Entertainment: " << budget.entertainment << std::endl
            << "| Clothing: " << budget.clothing << std::endl
            << "| Miscellaneous: " << budget.miscellaneous << std::endl;
}

void DisplayBudgetDifference(double difference, const std::string& category) {
  std::cout << "| " << category << ": ";
  if (difference < 0) {
    std::cout << "You were over budget by $" << (-difference) << "."
              << std::endl;
    return;
  }
  if (difference > 0) {
    std::cout << "You were under budget by $" << difference << "." << std::endl;
    return;
  }
  std::cout << "You matched your budget exactly." << std::endl;
}

void DisplayBudgetDifferences(const BudgetDifferences& differences) {
  std::cout << std::fixed << std::setprecision(2);  // setup std::cout

  std::cout << "Spending summary:" << std::endl;
  DisplayBudgetDifference(differences.housing, "Housing");
  DisplayBudgetDifference(differences.utilities, "Utilities");
  DisplayBudgetDifference(differences.household_expenses, "Household Expenses");
  DisplayBudgetDifference(differences.transportation, "Transportation");
  DisplayBudgetDifference(differences.food, "Food");
  DisplayBudgetDifference(differences.medical, "Medical");
  DisplayBudgetDifference(differences.insurance, "Insurance");
  DisplayBudgetDifference(differences.entertainment, "Entertainment");
  DisplayBudgetDifference(differences.clothing, "Clothing");
  DisplayBudgetDifference(differences.miscellaneous, "Miscellaneous");

  std::cout << "|" << std::endl << "| In total, ";
  const double total_difference = differences.total_difference;
  if (total_difference < 0) {
    std::cout << "you spent $" << (-total_difference) << " more than budgeted."
              << std::endl;
  } else if (total_difference > 0) {
    std::cout << "you spent $" << total_difference << " less than budgeted."
              << std::endl;
  } else {
    std::cout << "You spent exactly the amount that was budgeted." << std::endl;
  }

  std::cout << std::endl;
}

BudgetDifferences CompareBudgetToSpending(const MonthlyBudget& budget,
                                          const MonthlySpending& spending) {
  BudgetDifferences differences;

  differences.housing = budget.housing - spending.housing;
  differences.utilities = budget.utilities - spending.utilities;
  differences.household_expenses =
      budget.household_expenses - spending.household_expenses;
  differences.transportation = budget.transportation - spending.transportation;
  differences.food = budget.food - spending.food;
  differences.medical = budget.medical - spending.medical;
  differences.insurance = budget.insurance - spending.insurance;
  differences.entertainment = budget.entertainment - spending.entertainment;
  differences.clothing = budget.clothing - spending.clothing;
  differences.miscellaneous = budget.miscellaneous - spending.miscellaneous;

  double budget_total =
      budget.housing + budget.utilities + budget.household_expenses +
      budget.transportation + budget.food + budget.medical + budget.insurance +
      budget.entertainment + budget.clothing + budget.miscellaneous;

  double spending_total =
      spending.housing + spending.utilities + spending.household_expenses +
      spending.transportation + spending.food + spending.medical +
      spending.insurance + spending.entertainment + spending.clothing +
      spending.miscellaneous;

  differences.total_difference = budget_total - spending_total;

  return differences;
}

bool ValidateSpending(double spending) {
  if (spending < 0) {
    std::cout << "Spending cannot be negative." << std::endl << std::endl;
    return false;
  }

  return true;
}

}  // namespace customeraccounts
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::customeraccounts::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::customeraccounts::Run();
}
