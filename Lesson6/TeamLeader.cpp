// Copyright 2019 Michael Johnson

#include "TeamLeader.h"

#include <iomanip>
#include <iostream>

#include "../common.h"

namespace mjohnson {
namespace teamleader {

// FORWARD DECLARATIONS

TeamLeader PromptTeamLeaderInfo();
void DisplayTeamLeader(const TeamLeader& team_leader);
// ValidateName validates a user-entered name and returns true when the input is
// acceptable
bool ValidateName(std::string name);
// ValidateEmployeeNumber validates a user-entered employee number and returns
// true when the input is acceptable
bool ValidateEmployeeNumber(int number);
// ValidateEmployeeHireDate validates a user-entered hire date and returns true
// when the input is acceptable
bool ValidateEmployeeHireDate(std::string hire_date);
// ValidateProductionWorkerShift validates a user-entered shift and returns true
// when the input is acceptable
bool ValidateProductionWorkerShift(int shift);
// ValidateProductionWorkerShift validates a user-entered pay rate and returns
// true when the input is acceptable
bool ValidateProductionWorkerPayRate(double pay_rate);
// ValidateTeamLeaderBonus validates a user-entered bonus and returns true when
// the input is acceptable
bool ValidateTeamLeaderBonus(double bonus);
// ValidateTeamTrainingTime validates a user-entered training time and returns
// true when the input is acceptable
bool ValidateTeamLeaderTrainingTime(int training_time);

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    TeamLeader team_leader = PromptTeamLeaderInfo();
    DisplayTeamLeader(team_leader);
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UTILITY FUNCTIONS

TeamLeader PromptTeamLeaderInfo() {
  mjohnson::common::ClearScreen();

  TeamLeader t;

  t.set_name(mjohnson::common::RequestInput<std::string>(
      "What is the team leader's name? ", ValidateName));
  t.set_number(mjohnson::common::RequestInput<int>(
      "What is the team leader's employee ID? ", ValidateEmployeeNumber));
  t.set_hire_date(mjohnson::common::RequestInput<std::string>(
      "What is the team leader's hire date? ", ValidateEmployeeHireDate));
  t.set_shift(mjohnson::common::RequestInput<int>(
      "What shift is the employee on?\n[1] Day\n[2] Night\nEnter a choice: ",
      ValidateProductionWorkerShift));
  t.set_pay_rate(mjohnson::common::RequestInput<double>(
      "What is the team leader's pay rate? $",
      ValidateProductionWorkerPayRate));
  t.set_bonus(mjohnson::common::RequestInput<double>(
      "What is the team leader's monthly bonus? $", ValidateTeamLeaderBonus));
  t.set_required_training(mjohnson::common::RequestInput<int>(
      "How many hours of training is the team leader required to attend? ",
      ValidateTeamLeaderTrainingTime));
  t.set_completed_training(mjohnson::common::RequestInput<int>(
      "How many hours of training has the team leader attended? ",
      ValidateTeamLeaderTrainingTime));

  return t;
}

void DisplayTeamLeader(const TeamLeader& team_leader) {
  mjohnson::common::ClearScreen();

  std::cout << "========== TEAM LEADER ==========" << std::endl
            << "----- Employee Information -----" << std::endl
            << "Name: " << team_leader.name() << std::endl
            << "Employee ID: " << team_leader.number() << std::endl
            << "Hire date: " << team_leader.hire_date() << std::endl
            << "----- Production Worker Information -----" << std::endl
            << "Pay rate: $" << std::fixed << std::setprecision(2)
            << team_leader.pay_rate() << "/hr" << std::endl
            << "Shift: " << team_leader.shift_name() << std::endl
            << "----- Team Leader Information -----" << std::endl
            << "Monthly bonus: $" << team_leader.bonus() << std::endl
            << "Training completed: " << team_leader.completed_training()
            << " out of " << team_leader.required_training() << " hours"
            << std::endl;
}

bool ValidateName(std::string name) {
  if (name.empty()) {
    std::cout << "You must supply a name." << std::endl << std::endl;
    return false;
  }

  return true;
}

bool ValidateEmployeeNumber(int number) {
  if (number < 0) {
    std::cout << "Employee numbers cannot be negative." << std::endl
              << std::endl;
    return false;
  }

  return true;
}

bool ValidateEmployeeHireDate(std::string hire_date) {
  if (hire_date.empty()) {
    std::cout << "You must enter a hire date." << std::endl << std::endl;
    return false;
  }

  return true;
}

bool ValidateProductionWorkerShift(int shift) {
  if (shift != 1 && shift != 2) {
    std::cout << "Your choices are: 1, 2. " << shift
              << " is not a valid choice." << std::endl
              << std::endl;
    return false;
  }

  return true;
}

bool ValidateProductionWorkerPayRate(double pay_rate) {
  if (pay_rate == 0) {
    std::cout << "You must pay the production worker." << std::endl
              << std::endl;
    return false;
  }
  if (pay_rate < 0) {
    std::cout << "Pay can not be negative." << std::endl << std::endl;
    return false;
  }

  return true;
}

bool ValidateTeamLeaderBonus(double bonus) {
  if (bonus < 0) {
    std::cout << "The bonus cannot be negative." << std::endl << std::endl;
    return false;
  }

  return true;
}

bool ValidateTeamLeaderTrainingTime(int training_time) {
  if (training_time < 0) {
    std::cout << "Training time cannot be negative." << std::endl << std::endl;
    return false;
  }

  return true;
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() { return true; }

}  // namespace teamleader
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::teamleader::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::teamleader::Run();
}
