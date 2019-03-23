// Copyright 2019 Michael Johnson

#pragma once

#include <stdexcept>
#include <string>

namespace mjohnson {
namespace teamleader {

class Employee {
 private:
  // The employee's first and last name
  std::string name_ = "";
  // The employee's ID number
  int number_ = 0;
  // The employee's hire date
  std::string hire_date_ = "";

 public:
  Employee() = default;  // Default constructor
  Employee(int number, const std::string& name, const std::string& hire_date) {
    this->set_number(number);
    this->set_name(name);
    this->set_hire_date(hire_date);
  }

  void set_name(const std::string& name) { this->name_ = name; }
  std::string name() const { return this->name_; }

  void set_number(int number) {
    if (number < 0) {
      throw std::invalid_argument("number");
    }
    this->number_ = number;
  }
  int number() const { return this->number_; }

  void set_hire_date(const std::string& hire_date) {
    this->hire_date_ = hire_date;
  }
  std::string hire_date() const { return this->hire_date_; }
};

class ProductionWorker : public Employee {
 private:
  // The shift that the production worker works on. 1 = day, 2 = night
  int shift_ = 1;
  // The production worker's pay rate
  double pay_rate_ = 0l;

 public:
  ProductionWorker() = default;
  ProductionWorker(int number, const std::string& name,
                   const std::string& hire_date, int shift, double pay_rate)
      : Employee(number, name, hire_date) {
    this->set_shift(shift);
    this->set_pay_rate(pay_rate);
  }

  void set_shift(int shift) {
    if (shift != 1 && shift != 2) {
      throw std::invalid_argument("shift");
    }
    this->shift_ = shift;
  }
  int shift() const { return this->shift_; }
  std::string shift_name() const {
    const int shift = this->shift();
    if (shift == 1) {
      return "Day";
    }
    if (shift == 2) {
      return "Night";
    }
    return "Unknown";
  }

  void set_pay_rate(double pay_rate) {
    if (pay_rate < 0) {
      throw std::invalid_argument("pay_rate");
    }
    this->pay_rate_ = pay_rate;
  }
  double pay_rate() const { return this->pay_rate_; }
};

class TeamLeader : public ProductionWorker {
 private:
  // The team leader's monthly bonus
  double bonus_ = 0l;
  // The amount of training that the team leader is required to attend in a year
  int required_training_ = 0;
  // The amount of training that the team leader has attended this year
  int completed_training_ = 0;

 public:
  TeamLeader() = default;

  TeamLeader(int number, const std::string& name, const std::string& hire_date,
             int shift, double pay_rate, double bonus, int required_training,
             int completed_training)
      : ProductionWorker(number, name, hire_date, shift, pay_rate) {
    this->set_bonus(bonus);
    this->set_required_training(required_training);
    this->set_completed_training(completed_training);
  }

  void set_bonus(double bonus) {
    if (bonus < 0) {
      throw std::invalid_argument("bonus");
    }
    this->bonus_ = bonus;
  }
  double bonus() const { return this->bonus_; }

  void set_required_training(int required_training) {
    if (required_training < 0) {
      throw std::invalid_argument("required_training");
    }
    this->required_training_ = required_training;
  }
  int required_training() const { return this->required_training_; }

  void set_completed_training(int completed_training) {
    if (completed_training < 0) {
      throw std::invalid_argument("completed_training");
    }
    this->completed_training_ = completed_training;
  }
  int completed_training() const { return this->completed_training_; }
};

}  // namespace teamleader
}  // namespace mjohnson
