// Copyright 2019 Michael Johnson

#include <iostream>
#include <stdexcept>

#include "../common.h"

namespace mjohnson {
namespace linkedlist {

// FORWARD DECLARATIONS

// StateCapitals is a wrapper class for the state capitals map. It provides
// convenience functions to help with accessing the map.
class IntListItem {
 private:
  IntListItem* _next;
  int _value;

 public:
  explicit IntListItem(int value);

  void set_next(IntListItem* next);
  IntListItem* next() const;

  int value() const;
};

class IntLinkedList {
 private:
  IntListItem* _first;

  IntListItem* LastItem() const;
  IntListItem* ItemAt(size_t index) const;

 public:
  IntLinkedList();
  IntLinkedList(const IntLinkedList& other);  // Copy constructor
  ~IntLinkedList();

  int Get(size_t index) const;
  size_t Length() const;
  void Append(int v);
  void Insert(size_t index, int v);
  void Delete(size_t index);
  void Print();
};

bool ValidateMainMenuChoice(const std::string& choice);

void PromptAppend(IntLinkedList* list);
void PromptInsert(IntLinkedList* list);
void PromptDelete(IntLinkedList* list);

// MAIN FUNCTIONS
int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    auto list = new IntLinkedList();

    while (true) {
      mjohnson::common::ClearScreen();

      std::cout << "== List ==" << std::endl;
      list->Print();

      std::cout << "Options:" << std::endl
                << "[a] Append" << std::endl
                << "[i] Insert" << std::endl
                << "[d] Delete" << std::endl
                << "[c] Copy" << std::endl
                << "[q] Quit" << std::endl
                << std::endl;

      const auto choice = mjohnson::common::RequestInput<std::string>(
          "What would you like to do? ", ValidateMainMenuChoice);

      if (choice == "a") {
        PromptAppend(list);
      } else if (choice == "i") {
        PromptInsert(list);
      } else if (choice == "d") {
        PromptDelete(list);
      } else if (choice == "c") {
        auto copiedList =
            new IntLinkedList(*list);  // Call the copy constructor
        delete list;
        list = copiedList;  // Use the copy as our new list
      } else if (choice == "q") {
        break;
      } else {
        // We should never reach this
        throw std::invalid_argument("choice");
      }
    }
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UTILITY FUNCTIONS

bool ValidateMainMenuChoice(const std::string& choice) {
  if (choice != "a" && choice != "i" && choice != "d" && choice != "c" &&
      choice != "q") {
    std::cout << "Your choice must be a, i, or d." << std::endl << std::endl;
    return false;
  }

  return true;
}

void PromptAppend(IntLinkedList* list) {
  const auto num = mjohnson::common::RequestInput<int>(
      "What number would you like to append? ", nullptr);
  list->Append(num);
}

void PromptInsert(IntLinkedList* list) {
  size_t i = 0;
  bool first = true;
  do {
    if (!first) {
      std::cout << i << " is not a valid index." << std::endl << std::endl;
    } else {
      first = false;
    }

    i = mjohnson::common::RequestInput<size_t>(
        "Before what index would you like to insert your number? ", nullptr);
  } while (i > list->Length());

  const auto num = mjohnson::common::RequestInput<int>(
      "What number would you like to insert? ", nullptr);

  list->Insert(i, num);
}
void PromptDelete(IntLinkedList* list) {
  size_t i = 0;
  bool first = true;
  do {
    if (!first) {
      std::cout << i << " is not a valid index." << std::endl << std::endl;
    } else {
      first = false;
    }

    i = mjohnson::common::RequestInput<size_t>(
        "What index would you like to delete? ", nullptr);
  } while (i >= list->Length());

  list->Delete(i);
}

IntLinkedList::IntLinkedList() { this->_first = nullptr; }

IntLinkedList::IntLinkedList(const IntLinkedList& other) {
  IntListItem* other_item = other._first;
  if (other_item == nullptr) {
    // Other list is empty, initialize first and return
    this->_first = nullptr;
    return;
  }

  IntListItem* item = new IntListItem(other_item->value());
  this->_first = item;
  other_item = other_item->next();
  for (; other_item != nullptr; other_item = other_item->next()) {
    auto new_item = new IntListItem(other_item->value());
    item->set_next(new_item);
    item = new_item;
  }
}

IntLinkedList::~IntLinkedList() {
  // Free the memory associated with all of the items
  IntListItem* item = this->_first;
  while (item != nullptr) {
    IntListItem* next_item = item->next();
    delete item;
    item = next_item;
  }
}

IntListItem* IntLinkedList::LastItem() const {
  IntListItem* item = this->_first;
  if (item == nullptr) {
    return nullptr;
  }

  while (true) {
    IntListItem* next = item->next();
    if (next == nullptr) {
      // This is the last item
      return item;
    }
    item = next;
  }
}

IntListItem* IntLinkedList::ItemAt(size_t index) const {
  IntListItem* item = this->_first;
  if (item == nullptr) {
    // The list has no items
    throw std::out_of_range("index");
  }

  for (size_t i = 0; i < index; i++) {
    item = item->next();
    if (item == nullptr) {
      // The given index is beyond the end of the list
      throw std::out_of_range("index");
    }
  }

  return item;
}

int IntLinkedList::Get(size_t index) const {
  IntListItem* item = this->ItemAt(index);
  return item->value();
}

size_t IntLinkedList::Length() const {
  size_t counter = 0;
  for (IntListItem* item = this->_first; item != nullptr; item = item->next()) {
    counter++;
  }

  return counter;
}

void IntLinkedList::Append(int v) {
  auto new_item = new IntListItem(v);

  if (this->_first == nullptr) {
    // Special case: This is the first item to be added to this list
    this->_first = new_item;
    return;
  }

  IntListItem* old_last = this->LastItem();
  old_last->set_next(new_item);
}

void IntLinkedList::Insert(size_t index, int v) {
  auto item = new IntListItem(v);

  if (index == 0) {
    // Shortcut/special case: We're inserting at the beginning of the list
    IntListItem* old_first = this->_first;
    this->_first = item;
    item->set_next(old_first);  // It doesn't matter if old_first is nullptr
    return;
  }

  // After the operation, item_before will be the item before and item_after
  // will be the item after
  IntListItem* item_before = this->ItemAt(index - 1);
  const size_t length = this->Length();
  if (index == length) {
    // Special case: We're inserting at the end of the list
    // As such, we don't attempt to find the item after
    item_before->set_next(item);
    return;
  }

  IntListItem* item_after = this->ItemAt(index);
  item_before->set_next(item);
  item->set_next(item_after);
}

void IntLinkedList::Delete(size_t index) {
  if (index == 0) {
    // Special case: We're deleting the first item
    if (this->_first == nullptr) {
      // There is no first item
      throw std::out_of_range("index");
    }

    IntListItem* new_first = this->_first->next();
    delete this->_first;  // Clean up the memory taken by the deleted item
    this->_first = new_first;
    return;
  }

  const size_t length = this->Length();

  IntListItem* item_before = this->ItemAt(index - 1);
  IntListItem* item = this->ItemAt(index);

  if (index == (length - 1)) {
    // Special case: We're deleting the last item
    item_before->set_next(nullptr);
    delete item;  // Clean up the memory taken by that item
    return;
  }

  IntListItem* item_after = this->ItemAt(index + 1);
  item_before->set_next(item_after);

  delete item;  // Clean up the memory taken by the deleted item
}

void IntLinkedList::Print() {
  const size_t length = this->Length();
  if (length == 0) {
    std::cout << "The list is empty." << std::endl << std::endl;
  } else {
    size_t i = 0;
    for (IntListItem* item = this->_first; item != nullptr;
         item = item->next()) {
      std::cout << "[" << i << "] " << item->value() << std::endl;
      i++;
    }
    std::cout << std::endl;
  }
}

IntListItem::IntListItem(int value) : _value(value) { this->_next = nullptr; }

void IntListItem::set_next(IntListItem* next) { this->_next = next; }
IntListItem* IntListItem::next() const { return this->_next; }
int IntListItem::value() const { return this->_value; }

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or
// failure of those unit tests as a boolean.
bool RunUnitTests() { return true; }
}  // namespace linkedlist
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool run_unit_tests;
  if (!mjohnson::common::ParseArgs(argc, argv, &run_unit_tests)) {
    return 1;
  }

  if (run_unit_tests) {
    const bool result = mjohnson::linkedlist::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  return mjohnson::linkedlist::Run();
}

// Grade: 100
