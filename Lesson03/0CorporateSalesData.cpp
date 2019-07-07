// Copyright 2019 Michael Johnson

#include <iomanip>
#include <iostream>

#include "../common.h"

namespace mjohnson {
namespace salesdata {
// STRUCT DEFINITIONS
struct DivisionSales {
  std::string name;

  double q1_sales;
  double q2_sales;
  double q3_sales;
  double q4_sales;

  double total_sales;
  double average_sales;
};

// FORWARD DECLARATIONS

// PromptSales prompts the user to fill in the sales data for a division.
void PromptSales(DivisionSales* sales);
// DisplaySalesSummary displays the summary for a particular region's sales
void DisplaySalesSummary(const DivisionSales& sales);

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    mjohnson::common::ClearScreen();

    DivisionSales east;
    east.name = "East";
    DivisionSales west;
    west.name = "West";
    DivisionSales north;
    north.name = "North";
    DivisionSales south;
    south.name = "South";

    PromptSales(&north);
    PromptSales(&east);
    PromptSales(&south);
    PromptSales(&west);

    mjohnson::common::ClearScreen();

    DisplaySalesSummary(east);
    DisplaySalesSummary(west);
    DisplaySalesSummary(north);
    DisplaySalesSummary(south);
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() { return true; }

// UTILITY FUNCTION DEFINITIONS

void DisplaySalesSummary(const DivisionSales& sales) {
  std::cout << std::fixed << std::setprecision(2) << "Division: " << sales.name
            << std::endl
            << "| Q1 sales: $" << sales.q1_sales << std::endl
            << "| Q2 sales: $" << sales.q2_sales << std::endl
            << "| Q3 sales: $" << sales.q3_sales << std::endl
            << "| Q4 sales: $" << sales.q4_sales << std::endl
            << "|" << std::endl
            << "| Total: $" << sales.total_sales << std::endl
            << "| Average: $" << sales.average_sales << " per quarter"
            << std::endl
            << std::endl;
}

bool ValidateSalesFigure(double sales);

void PromptSales(DivisionSales* sales) {
  mjohnson::common::ClearScreen();
  std::cout << "========== Sales for the " << sales->name
            << " division ==========" << std::endl
            << std::endl;

  sales->q1_sales = mjohnson::common::RequestInput<double>(
      "What were the first quarter sales? $", ValidateSalesFigure);
  sales->q2_sales = mjohnson::common::RequestInput<double>(
      "What were the second quarter sales? $", ValidateSalesFigure);
  sales->q3_sales = mjohnson::common::RequestInput<double>(
      "What were the third quarter sales? $", ValidateSalesFigure);
  sales->q4_sales = mjohnson::common::RequestInput<double>(
      "What were the fourth quarter sales? $", ValidateSalesFigure);

  sales->total_sales =
      sales->q1_sales + sales->q2_sales + sales->q3_sales + sales->q4_sales;
  sales->average_sales = sales->total_sales / 4l;
}

bool ValidateSalesFigure(double sales) {
  if (sales < 0) {
    std::cout << "Quarterly sales cannot be negative." << std::endl
              << std::endl;
    return false;
  }

  return true;
}
}  // namespace salesdata
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::salesdata::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::salesdata::Run();
}
