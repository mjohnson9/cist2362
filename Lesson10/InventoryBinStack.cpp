// Copyright 2019 Michael Johnson

#include <ctime>
#include <iostream>
#include <memory>

#include "../common.h"

namespace mjohnson {
namespace inventorybinstack {

// FORWARD DECLARATIONS

/**
 * Represents an inventory item in the inventory bin stack.
 */
class InventoryItem {
 private:
  /**
   * The part's serial number.
   */
  uint64_t serial_number_;

  /**
   * The time that the part was manufactured (accurate only to the day).
   */
  std::time_t manufacture_date_;

  /**
   * The part's lot number.
   */
  uint64_t lot_number_;

 public:
  /**
   * Constructs a new inventory item. The new inventory item will have a serial
   * number of 0, a lot number of 0, and a manufacture time of 1970-01-01.
   */
  InventoryItem();

  /**
   * Retrieves the serial number.
   * @return The serial number.
   */
  uint64_t SerialNumber() const { return this->serial_number_; }
  /**
   * Sets the serial number.
   * @param serial_number The new serial number.
   */
  void SetSerialNumber(uint64_t serial_number) {
    this->serial_number_ = serial_number;
  }

  /**
   * Retrieves the manufacture date.
   * @return The manufacture date.
   */
  std::time_t ManufactureDate() const { return this->manufacture_date_; }
  /**
   * Retrieves the manufacture date as a "YYYY-MM-DD" string.
   * @return The manufacture date formatted as a "YYYY-MM-DD" string.
   */
  std::string ManufactureDateString() const;
  /**
   * Sets the manufacture date.
   * @param manufacture_date The new manufacture date.
   */
  void SetManufactureDate(std::time_t manufacture_date) {
    this->manufacture_date_ = manufacture_date;
  }

  /**
   * Retrieves the lot number.
   * @return The lot number.
   */
  uint64_t LotNumber() const { return this->lot_number_; }
  /**
   * Sets the lot number.
   * @param lot_number The new lot number.
   */
  void SetLotNumber(uint64_t lot_number) { this->lot_number_ = lot_number; }

  /**
   * Prints the part's information to std::cout.
   */
  void Print();
};

/**
 * Creates a dynamic stack. This stack grows and shrinks to accomodate any
 * number of elements, up to the limits of allocatable memory.
 */
template <typename T>
class DynamicStack {
 private:
  /**
   * Item represents a single stack item. It's necessary to maintain the linked
   * list-like behavior of the stack.
   */
  struct Item {
    Item* next;
    T value;
  };

  /**
   * The item on the top of the stack. All items underneath can be found by
   * accessing this item's next value.
   */
  Item* top_;

 public:
  /**
   * Constructs a dynamic stack.
   */
  DynamicStack();
  /**
   * Destructs a dynamic stack.
   */
  ~DynamicStack();

  /**
   * Pushes a value onto the top of the stack.
   * @param value The value to push onto the top of the stack.
   */
  void Push(T value);
  /**
   * Pops a value off of the top of the stack.
   * @return The topmost value on the stack.
   */
  T Pop();

  /**
   * Retrieves the current size of the stack.
   * @return The size of the stack.
   */
  size_t Size();
  /**
   * Checks whether the stack is empty.
   * @return True if the stack is empty, false otherwise.
   */
  bool IsEmpty();
};

/**
 * The type for an inventory item stack.
 *
 * Note: We use a reference counting pointer because pointer ownership is
 * unclear in this case.
 */
using DynamicInventoryItemStack = DynamicStack<std::shared_ptr<InventoryItem>>;

/**
 * Takes the user through the steps of adding an item to the inventory stack.
 * Blocks until the process is complete.
 * @param stack The stack to operate on.
 */
void AddItemMenu(DynamicInventoryItemStack* stack);
/**
 * Displays the top most item on the inventory stack to the user. Blocks until
 * the user decides to continue.
 * @param stack The stack to operate on.
 */
void TakeItemMenu(DynamicInventoryItemStack* stack);

// MAIN FUNCTIONS

int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  DynamicInventoryItemStack stack;
  std::string message;

  while (true) {
    mjohnson::common::ClearScreen();

    if (!message.empty()) {
      std::cout << message << std::endl << std::endl;
      message = "";
    }

    std::cout << "[A]dd an item | [T]ake an item | [E]xit" << std::endl;

    auto original_menu_choice =
        mjohnson::common::RequestInput<std::string>("", nullptr);

    std::string menu_choice(original_menu_choice);
    mjohnson::common::TrimString(&menu_choice);
    mjohnson::common::LowerString(&menu_choice);

    if (menu_choice == "a") {
      AddItemMenu(&stack);
      continue;
    }
    if (menu_choice == "t") {
      TakeItemMenu(&stack);
      continue;
    }
    if (menu_choice == "e") {
      break;
    }

    message = "\"" + original_menu_choice + "\" is not a valid option.";
  }

  mjohnson::common::ClearScreen();
  if (!stack.IsEmpty()) {
    std::cout << "You had the following items remaining in the inventory stack:"
              << std::endl
              << std::endl;
    bool first = true;

    while (!stack.IsEmpty()) {
      if (!first) {
        std::cout << "--------------------" << std::endl;
      } else {
        first = false;
      }

      auto item = stack.Pop();
      item->Print();
    }
  } else {
    std::cout << "You had no items remaining in the inventory stack."
              << std::endl;
  }

  return 0;
}

// DEFINITIONS

void AddItemMenu(DynamicInventoryItemStack* stack) {
  mjohnson::common::ClearScreen();

  std::cout << "========== ADD PART ==========" << std::endl << std::endl;

  auto item = std::shared_ptr<InventoryItem>(new InventoryItem());

  auto serial_number = mjohnson::common::RequestInput<uint64_t>(
      "What is the part's serial number? ", nullptr);
  item->SetSerialNumber(serial_number);

  auto lot_number = mjohnson::common::RequestInput<uint64_t>(
      "What is the part's lot number? ", nullptr);
  item->SetLotNumber(lot_number);

  while (true) {
    auto date_string = mjohnson::common::RequestInput<std::string>(
        "What is the part's manufacture date? (Use YYYY-MM-DD format) ",
        nullptr);

    tm manufacture_date_tm = {};

    if (strptime(date_string.c_str(), "%y-%m-%d", &manufacture_date_tm) ==
        nullptr) {
      std::cout << std::endl
                << "Unable to parse \"" << date_string
                << "\". Please use the format YYYY-MM-DD." << std::endl
                << std::endl;
      continue;
    }

    item->SetManufactureDate(mktime(&manufacture_date_tm));
    break;
  }

  stack->Push(item);
}

void TakeItemMenu(DynamicInventoryItemStack* stack) {
  mjohnson::common::ClearScreen();

  if (!stack->IsEmpty()) {
    std::cout << "========== TAKE ITEM =========" << std::endl
              << std::endl
              << "You've taken the following item:" << std::endl;

    auto item = stack->Pop();
    item->Print();
  } else {
    std::cout << "========== TAKE ITEM =========" << std::endl
              << std::endl
              << "There are no items to take." << std::endl;
  }

  std::cout << std::endl << "Press enter to continue." << std::endl;

  std::string discard;
  std::getline(std::cin, discard);
}

InventoryItem::InventoryItem() {
  this->serial_number_ = 0;
  this->lot_number_ = 0;

  const char* kDefaultDate = "1970-01-01";
  tm default_time_tm = {};

  if (strptime(kDefaultDate, "%Y-%m-%d", &default_time_tm) == nullptr) {
    throw std::logic_error("unable to parse default manufacture date");
  }
  this->manufacture_date_ = mktime(&default_time_tm);
}

void InventoryItem::Print() {
  std::cout << "Serial number: " << this->SerialNumber() << std::endl
            << "Lot number: " << this->LotNumber() << std::endl
            << "Manufacture date: " << this->ManufactureDateString()
            << std::endl;
}

std::string InventoryItem::ManufactureDateString() const {
  const size_t kBufSize = 128;
  char* buf = new char[kBufSize];

  std::time_t manufacture_date = this->ManufactureDate();

  tm manufacture_date_tm = {};

  localtime_r(&manufacture_date, &manufacture_date_tm);

  size_t buf_used = strftime(buf, kBufSize, "%F", &manufacture_date_tm);

  return std::string(buf, buf_used);
}

template <typename T>
DynamicStack<T>::DynamicStack() {
  this->top_ = nullptr;
}

template <typename T>
DynamicStack<T>::~DynamicStack() {
  auto item = this->top_;
  while (item != nullptr) {
    auto next_item = item->next;
    delete item;
    item = next_item;
  }
}

template <typename T>
void DynamicStack<T>::Push(T value) {
  auto new_item = new DynamicStack<T>::Item();
  new_item->next = this->top_;
  new_item->value = value;

  this->top_ = new_item;
}

template <typename T>
T DynamicStack<T>::Pop() {
  if (this->IsEmpty()) {
    throw std::length_error("stack is empty");
  }

  auto old_top = this->top_;
  T value = old_top->value;

  this->top_ = old_top->next;

  delete old_top;

  return value;
}

template <typename T>
bool DynamicStack<T>::IsEmpty() {
  return (this->top_ == nullptr);
}

template <typename T>
size_t DynamicStack<T>::Size() {
  size_t size = 0;

  auto item = this->top_;
  while (item != nullptr) {
    size += 1;

    auto old_item = item;
    item = old_item->next;
  }

  return size;
}

// UNIT TESTING

/**
 * Runs the program's unit tests. Returns the success or failure of those unit
 * tests.
 * @return True if all unit tests passed, false otherwise.
 */
bool RunUnitTests() { return true; }
}  // namespace inventorybinstack
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::inventorybinstack::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::inventorybinstack::Run();
}

// Grade: 100
