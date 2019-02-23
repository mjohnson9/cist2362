// Copyright 2019 Michael Johnson

#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

namespace mjohnson {
namespace ship {

class Ship {
 private:
  // This ship's name
  std::string name_;
  // The year that this ship was built
  std::string year_built_;

  // Disable the default constructor
  Ship() = default;

 public:
  Ship(const std::string& name, const std::string& year_built) {
    this->set_name(name);
    this->set_year_built(year_built);
  }

  // We have to make the destructors virtual to satisfy the compiler
  virtual ~Ship() = default;

  std::string name() const { return this->name_; }
  void set_name(const std::string& name) { this->name_ = name; }

  std::string year_built() const { return this->year_built_; }
  void set_year_built(const std::string& year_built) {
    this->year_built_ = year_built;
  }

  virtual void print() const {
    std::cout << this->name() << " (" << this->year_built() << ")" << std::endl;
  }
};

class CruiseShip : public Ship {
 private:
  // The ship's passenger capacity
  int max_passengers_;

 public:
  CruiseShip(const std::string& name, const std::string& year_built,
             int max_passengers)
      : Ship(name, year_built) {
    this->set_max_passengers(max_passengers);
  }

  // We have to make the destructors virtual to satisfy the compiler
  ~CruiseShip() override = default;

  int max_passengers() const { return this->max_passengers_; }
  void set_max_passengers(int max_passengers) {
    this->max_passengers_ = max_passengers;
  }

  void print() const override {
    std::cout << this->name() << ", maximum of " << this->max_passengers()
              << " passengers" << std::endl;
  }
};

class CargoShip : public Ship {
 private:
  // The maximum amount of cargo the ship can carry (in tons)
  int cargo_capacity_;

 public:
  CargoShip(const std::string& name, const std::string& year_built,
            int cargo_capacity)
      : Ship(name, year_built) {
    this->set_cargo_capacity(cargo_capacity);
  }

  // We have to make the destructors virtual to satisfy the compiler
  ~CargoShip() override = default;

  int cargo_capacity() const { return this->cargo_capacity_; }
  void set_cargo_capacity(int cargo_capacity) {
    this->cargo_capacity_ = cargo_capacity;
  }

  void print() const override {
    std::cout << this->name() << ", cargo capacity of "
              << this->cargo_capacity() << " tons" << std::endl;
  }
};

}  // namespace ship
}  // namespace mjohnson
