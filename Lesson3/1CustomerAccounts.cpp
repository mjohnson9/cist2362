// Copyright 2019 Michael Johnson

#include <exception>
#include <iomanip>
#include <iostream>
#include <locale>

#include "../common.h"

namespace mjohnson {
namespace customeraccounts {
// EXCEPTION DEFINITIONS

// UserCancelledException is an exception to be thrown whenever the user cancels
// their menu interaction, expecting to go back to the next menu up
class UserCancelledException : public std::exception {
 private:
  // The menu that the user cancelled interaction from
  char* from_;
  // The length of from_ (we keep this to avoid buffer overflows)
  size_t from_len_;
  // The message returned by what()
  char* what_message_;
  // The length of the message returned by what() (we keep this to avoid buffer
  // overflows)
  size_t what_message_len_;

  // Init moves the duplicate work of the constructors into a single private
  // function
  void Init(const char* from) {
    // copy "from" into our own c-string so that the caller freeing their
    // c-string doesn't invalidate our pointer
    this->from_len_ = strlen(from);
    this->from_ = new char[this->from_len_ + 1];
    strncpy(this->from_, from, this->from_len_);
    this->from_[this->from_len_] = '\0';  // Set null terminator

    // create what_message_
    const char* what_message_begin = "user interaction cancelled from ";
    const size_t what_message_begin_len = strlen(what_message_begin);

    this->what_message_len_ = what_message_begin_len + this->from_len_;
    this->what_message_ = new char[this->what_message_len_ + 1];

    // Copy this->from_ into this->what_message_ beginning at the end of
    // what_message_begin
    strncpy(&this->what_message_[what_message_begin_len], this->from_,
            this->from_len_);

    this->what_message_[this->what_message_len_] = '\0';  // Set null terminator
  }

 public:
  // Make these constructors explicit so that there is no implicit cast from
  // strings to this exception

  // Constructs a UserCancelledException from a c-string
  explicit UserCancelledException(const char* from) { this->Init(from); }

  // Constructs a UserCancelledException from a std::string
  explicit UserCancelledException(const std::string& from) {
    this->Init(from.c_str());
  }

  // Copy constructor for UserCancelledException
  UserCancelledException(const UserCancelledException& other) {
    this->Init(other.From());
  }

  // This destructor frees internal resources to prevent memory leaks
  ~UserCancelledException() override {
    delete[] this->from_;
    delete[] this->what_message_;
  }

  // Returns the menu that was cancelled from
  const char* From() const noexcept { return this->from_; }

  // Returns the what-message that describes the exception
  const char* what() const noexcept override { return this->what_message_; }
};

// STRUCT DEFINITIONS

// Customer is a structure to contain the data of a single customer.
struct Customer {
  // Name
  std::string first_name;
  std::string last_name;

  // Address
  std::string street_address;
  std::string city;
  std::string state;
  std::string zip_code;

  // Phone
  std::string phone_number;
};

// CustomerAccount is a structure to contain the data regarding a single
// customer account as well as the account's owner.
struct CustomerAccount {
  // Customer
  Customer owner;

  // Account details
  double balance;
  std::string last_payment;
};

// FUNCTION FORWARD DECLARATIONS

// AccountsMainMenu presents the main menu to the user and allows them to select
// an account to work with
size_t AccountsMainMenu(CustomerAccount* customer_accounts[],
                        size_t num_accounts);
// AccountEditMenu presents a user with the ability to edit an account
void AccountEditMenu(CustomerAccount** customer_account_ref);
// DisplayAccountEditingHeader displays the header for editing an account
void DisplayAccountEditingHeader(CustomerAccount* customer_account);
// AccountEditNameMenu presents a user with the ability to edit an account's
// name
void AccountEditNameMenu(CustomerAccount* customer_account);
// AccountEditAddressMenu presents a user with the ability to edit an account's
// address
void AccountEditAddressMenu(CustomerAccount* customer_account);
// AccountEditPhoneMenu presents a user with the ability to edit an account's
// phone number
void AccountEditPhoneMenu(CustomerAccount* customer_account);
// AccountEditBalanceMenu presents a user with the ability to edit an account's
// balance
void AccountEditBalanceMenu(CustomerAccount* customer_account);
// AccountCreationMenu presents a user with the ability to create an account
void AccountCreationMenu(CustomerAccount** customer_account_ref);
// DisplayAccountSummary displays a one-line summary of an account
void DisplayAccountSummary(size_t num, CustomerAccount* customer_account);
// ValidateNumAccounts validates that the given number of accounts is a number
// of accounts that can actually be used.
bool ValidateNumAccounts(size_t num_accounts);
// ValidateEditingAnswer validates that the given answer is a number within the
// given choices for the editing menu.
bool ValidateEditingAnswer(int32_t choice);
// ValidateAccountBalance validates that the given answer is a valid account
// balance.
bool ValidateAccountBalance(double balance);
// ValidateStringNotEmpty validates that the given string is not empty (after
// removing whitespace characters)
bool ValidateStringNotEmpty(std::string str);

// MAIN FUNCTIONS

int MainMenu() {
  mjohnson::common::ClearScreen();

  auto num_customer_accounts = mjohnson::common::RequestInput<size_t>(
      "How many accounts will you be working with? ", ValidateNumAccounts);
  auto customer_accounts = new CustomerAccount*[num_customer_accounts]();

  do {
    const size_t selected_account =
        AccountsMainMenu(customer_accounts, num_customer_accounts);

    try {
      AccountEditMenu(&customer_accounts[selected_account]);
    } catch (const UserCancelledException& e) {
      // The user left that account editing menu
      mjohnson::common::ClearScreen();
      continue;
    }
  } while (true);

  delete[] customer_accounts;
  return 0;
}

int Run() {
  // Add the computer's locale to cout. This lets us do thousands separators and
  // decimal points in the locale of the user.
  std::cout.imbue(std::locale(""));

  do {
    try {
      const int main_menu_status = MainMenu();
      if (main_menu_status != 0) {
        // The main menu returned a bad exit code; we should too
        return main_menu_status;
      }
    } catch (const UserCancelledException& e) {
      // The user left the accounts main menu
      mjohnson::common::ClearScreen();
      continue;
    }
  } while (mjohnson::common::RequestContinue());

  return 0;
}

// UNIT TESTING

// RunUnitTests runs the program's unit tests and returns the success or failure
// of those unit tests as a boolean.
bool RunUnitTests() { return true; }

// UTILITY FUNCTION DEFINITIONS

size_t AccountsMainMenu(CustomerAccount* customer_accounts[],
                        size_t num_accounts) {
  mjohnson::common::ClearScreen();

  std::cout << "========== ACCOUNTS ==========" << std::endl;

  for (size_t i = 0; i < num_accounts; i++) {
    CustomerAccount* customer_account = customer_accounts[i];
    DisplayAccountSummary(i + 1, customer_account);
  }

  std::cout << "[0] Exit" << std::endl << std::endl;

  const auto choice = mjohnson::common::RequestInput<size_t>(
      "Which account would you like to work with? ", nullptr);
  if (choice == 0) {
    throw UserCancelledException("account selection");
  }

  return choice - 1;
}

void AccountEditMenu(CustomerAccount** customer_account_ref) {
  CustomerAccount* customer_account =
      *customer_account_ref;  // Dereference the outside pointer to get a
                              // pointer to the actual object
  if (customer_account == nullptr) {
    return AccountCreationMenu(customer_account_ref);
  }

  do {
    mjohnson::common::ClearScreen();

    DisplayAccountEditingHeader(customer_account);

    std::cout.unsetf(std::ios_base::floatfield);  // clear std::fixed

    std::cout << "[1] Change name" << std::endl
              << "[2] Change address" << std::endl
              << "[3] Change phone number" << std::endl
              << "[4] Change balance" << std::endl
              << "[0] Return to main menu" << std::endl;

    auto menu_choice = mjohnson::common::RequestInput<int32_t>(
        "Please choose a menu item: ", ValidateEditingAnswer);

    switch (menu_choice) {
      case 0:
        return;
      case 1:
        AccountEditNameMenu(customer_account);
      case 2:
        AccountEditAddressMenu(customer_account);
      case 3:
        AccountEditPhoneMenu(customer_account);
      case 4:
        AccountEditBalanceMenu(customer_account);
    }
    std::cout << menu_choice << std::endl;
  } while (true);
}

void AccountEditNameMenu(CustomerAccount* customer_account) {
  mjohnson::common::ClearScreen();

  DisplayAccountEditingHeader(customer_account);

  customer_account->owner.first_name =
      mjohnson::common::RequestInput<std::string>(
          "What is the account owner's new first name? ",
          ValidateStringNotEmpty);
  customer_account->owner.last_name =
      mjohnson::common::RequestInput<std::string>(
          "What is the account owner's new last name? ",
          ValidateStringNotEmpty);
}

void AccountEditAddressMenu(CustomerAccount* customer_account) {
  mjohnson::common::ClearScreen();

  DisplayAccountEditingHeader(customer_account);

  customer_account->owner.street_address =
      mjohnson::common::RequestInput<std::string>(
          "What is the new street address? ", ValidateStringNotEmpty);
  customer_account->owner.city = mjohnson::common::RequestInput<std::string>(
      "What city does the account owner now live in? ", ValidateStringNotEmpty);
  customer_account->owner.state = mjohnson::common::RequestInput<std::string>(
      "What state does the account owner now live in? ",
      ValidateStringNotEmpty);
  customer_account->owner.zip_code =
      mjohnson::common::RequestInput<std::string>(
          "What is the ZIP code where the account owner now lives? ",
          ValidateStringNotEmpty);
}

void AccountEditPhoneMenu(CustomerAccount* customer_account) {
  mjohnson::common::ClearScreen();

  DisplayAccountEditingHeader(customer_account);

  customer_account->owner.phone_number =
      mjohnson::common::RequestInput<std::string>(
          "What is the account owner's new phone number? ",
          ValidateStringNotEmpty);
}

void AccountEditBalanceMenu(CustomerAccount* customer_account) {
  mjohnson::common::ClearScreen();

  DisplayAccountEditingHeader(customer_account);

  customer_account->balance = mjohnson::common::RequestInput<double>(
      "What is the new balance? ", ValidateAccountBalance);
  customer_account->last_payment = mjohnson::common::RequestInput<std::string>(
      "When was the last payment? ", ValidateStringNotEmpty);
}

void DisplayAccountEditingHeader(CustomerAccount* customer_account) {
  std::cout << "========== EDITING ACCOUNT ==========" << std::endl
            << "Name: " << customer_account->owner.first_name << " "
            << customer_account->owner.last_name << std::endl
            << "Phone number: " << customer_account->owner.phone_number
            << std::endl
            << "Address:" << std::endl
            << customer_account->owner.street_address << std::endl
            << customer_account->owner.city << ", "
            << customer_account->owner.state << "  "
            << customer_account->owner.zip_code << std::endl
            << std::endl
            << "Account balance: " << std::fixed << std::setprecision(2)
            << customer_account->balance << std::endl
            << "Last payment: " << customer_account->last_payment << std::endl
            << std::endl;
}

void AccountCreationMenu(CustomerAccount** customer_account_ref) {
  {  // Check that the account is actually null in a separate scope to reduce
     // pollution
    CustomerAccount* customer_account =
        *customer_account_ref;  // Dereference the outside point to get a
                                // pointer to the actual object
    if (customer_account != nullptr) {
      throw std::invalid_argument("customer_account_ref");
    }
  }

  mjohnson::common::ClearScreen();
  std::cout << "========== CREATING AN ACCOUNT ==========" << std::endl;

  auto customer_account = new CustomerAccount;

  customer_account->owner.first_name =
      mjohnson::common::RequestInput<std::string>(
          "What is the account owner's first name? ", ValidateStringNotEmpty);
  customer_account->owner.last_name =
      mjohnson::common::RequestInput<std::string>(
          "What is the account owner's last name? ", ValidateStringNotEmpty);
  customer_account->owner.street_address =
      mjohnson::common::RequestInput<std::string>(
          "What is the account owner's street address? ",
          ValidateStringNotEmpty);
  customer_account->owner.city = mjohnson::common::RequestInput<std::string>(
      "What city does the account owner live in? ", ValidateStringNotEmpty);
  customer_account->owner.state = mjohnson::common::RequestInput<std::string>(
      "What state does the account owner live in? ", ValidateStringNotEmpty);
  customer_account->owner.zip_code =
      mjohnson::common::RequestInput<std::string>(
          "What is the ZIP code where the account owner lives? ",
          ValidateStringNotEmpty);
  customer_account->owner.phone_number =
      mjohnson::common::RequestInput<std::string>(
          "What is the account owner's phone number? ", ValidateStringNotEmpty);
  customer_account->balance = mjohnson::common::RequestInput<double>(
      "What is the account's starting balance? ", ValidateAccountBalance);
  customer_account->last_payment = mjohnson::common::RequestInput<std::string>(
      "When was the last payment on the account? ", ValidateStringNotEmpty);

  *customer_account_ref = customer_account;
}

void DisplayAccountSummary(size_t num, CustomerAccount* customer_account) {
  if (customer_account == nullptr) {
    std::cout << "[" << num << "] No account created" << std::endl;
    return;
  }
  std::cout << "[" << num << "] " << customer_account->owner.first_name << " "
            << customer_account->owner.last_name << " - $" << std::fixed
            << std::setprecision(2) << customer_account->balance << std::endl;

  std::cout.unsetf(std::ios_base::floatfield);  // clear std::fixed
}

bool ValidateNumAccounts(size_t num_accounts) {
  if (num_accounts < 10) {
    std::cout << "There must be at least 10 accounts." << std::endl;
    return false;
  }

  return true;
}

bool ValidateEditingAnswer(int32_t choice) {
  if (!(choice >= 0 && choice <= 4)) {
    std::cout << choice
              << " is not a valid menu option. Please choose one of the "
                 "displayed menu options."
              << std::endl
              << std::endl;
    return false;
  }

  return true;
}

bool ValidateStringNotEmpty(std::string str) {
  mjohnson::common::TrimString(&str);

  if (str.length() < 1) {
    std::cout << "The input cannot be empty." << std::endl << std::endl;
    return false;
  }

  return true;
}

bool ValidateAccountBalance(double balance) {
  if (balance < 0) {
    std::cout << "The account balance can not be negative." << std::endl
              << std::endl;
    return false;
  }

  return true;
}
}  // namespace customeraccounts
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  std::cout << std::isspace('\n') << std::endl;
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
