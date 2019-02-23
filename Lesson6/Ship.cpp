// Copyright 2019 Michael Johnson

#include "Ship.h"

#include <functional>  // NOLINT(build/include)
#include <iomanip>
#include <iostream>
#include <random>

#include "../common.h"

namespace mjohnson {
namespace ship {

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  std::default_random_engine generator;

  std::uniform_int_distribution<int> random_ship_distribution(1, 3);
  auto random_ship_type = std::bind(random_ship_distribution, generator);

  std::uniform_int_distribution<int> random_year_distribution(1970, 2018);
  auto random_ship_year = std::bind(random_year_distribution, generator);

  std::uniform_int_distribution<int> random_passenger_capacity_distribution(
      150, 2000);
  auto random_passenger_capacity =
      std::bind(random_passenger_capacity_distribution, generator);

  std::uniform_int_distribution<int> random_cargo_capacity_distribution(15000,
                                                                        25000);
  auto random_cargo_capacity =
      std::bind(random_cargo_capacity_distribution, generator);

  do {
    const size_t NUM_SHIPS = 10;
    Ship* ships[NUM_SHIPS];

    std::cout << NUM_SHIPS << " RANDOMLY GENERATED SHIPS:" << std::endl;

    for (auto& ship : ships) {
      const int ship_type = random_ship_type();
      const std::string ship_year = std::to_string(random_ship_year());
      switch (ship_type) {
        case 1:
          ship = new Ship("Ship Name", ship_year);
          break;
        case 2:
          ship = new CruiseShip("CruiseShip Name", ship_year,
                                random_passenger_capacity());
          break;
        case 3:
          ship = new CargoShip("CargoShip Name", ship_year,
                               random_cargo_capacity());
          break;
      }
    }

    for (const auto ship : ships) {
      ship->print();
    }

    for (auto ship : ships) {
      delete ship;
    }

    std::cout << std::endl;
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() { return true; }

}  // namespace ship
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::ship::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::ship::Run();
}
