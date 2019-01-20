// Copyright 2019 Michael Johnson

#include <iostream>

#include "../common.h"

namespace mjohnson {
namespace namearranger {
void RequestName(const std::string& prompt, char* buffer, size_t buffer_size);

void Run() {
  do {
    static const size_t MAX_NAME_SIZE = 0xFF;

    char first_name[MAX_NAME_SIZE];
    first_name[MAX_NAME_SIZE - 1] = '\0';
    char middle_name[MAX_NAME_SIZE];
    middle_name[MAX_NAME_SIZE - 1] = '\0';
    char last_name[MAX_NAME_SIZE];
    last_name[MAX_NAME_SIZE - 1] = '\0';

    RequestName("What is your first name? ", first_name, MAX_NAME_SIZE);
    const size_t first_name_len = strlen(first_name);

    RequestName("What is your middle name? ", middle_name, MAX_NAME_SIZE);
    const size_t middle_name_len = strlen(middle_name);

    RequestName("What is your last name? ", last_name, MAX_NAME_SIZE);
    const size_t last_name_len = strlen(last_name);

    mjohnson::common::ClearScreen();

    std::cout << std::endl;

    const size_t assembled_name_len = last_name_len + 2 /* comma space */ +
                                      first_name_len + 1 /* space */ +
                                      middle_name_len + 1 /*null terminator */;
    char assembled_name[assembled_name_len];
    assembled_name[assembled_name_len - 1] = '\0';  // Terminate with null

    {  // New scope to reduce clutter
      for (size_t i = 0; i < last_name_len; i++) {
        assembled_name[i] = last_name[i];
      }
      assembled_name[last_name_len] = ',';
      assembled_name[last_name_len + 1] = ' ';
    }
    {
      const size_t offset = last_name_len + 2;
      for (size_t i = 0; i < first_name_len; i++) {
        assembled_name[offset + i] = first_name[i];
      }
      assembled_name[offset + first_name_len] = ' ';
    }
    {
      const size_t offset = last_name_len + 2 + first_name_len + 1;
      for (size_t i = 0; i < middle_name_len; i++) {
        assembled_name[offset + i] = middle_name[i];
      }
    }

    std::cout << assembled_name << std::endl << std::endl;
  } while (mjohnson::common::RequestContinue());
}

void TrimSpaces(char* buffer, size_t buffer_size);

void RequestName(const std::string& prompt, char* buffer, size_t buffer_size) {
  bool valid = false;
  do {
    mjohnson::common::ClearScreen();

    std::cout << prompt;
    std::cin.getline(buffer, buffer_size);

    TrimSpaces(buffer, buffer_size);

    valid = (strlen(buffer) > 0);
  } while (!valid);
}

void TrimSpaces(char* buffer, size_t buffer_size) {
  bool last_was_space = true;
  size_t last_space_at = 0;
  for (size_t i = 0; buffer[i] != '\0' && i < buffer_size; i++) {
    bool is_space = (buffer[i] == ' ');

    if (is_space && last_was_space) {
      for (size_t j = i; buffer[j] != '\0' && j < buffer_size; j++) {
        buffer[j] = buffer[j + 1];  // Shift everything left one
      }

      i -= 1;  // We've just removed a character from the array; step backwards
    }

    last_was_space = is_space;
    if (last_was_space) {
      last_space_at = i;
    }
  }

  if (last_was_space) {  // The character array ended with a space
    buffer[last_space_at] = '\0';
  }
}

// TestCase represents a unit test case. It contains a string to be tested and
// the expected output of the CountWords function.
struct TestCase {
  // The string to be tested
  std::string original;
  // The expected output of CountWords
  std::string expected;
};

bool RunUnitTests() {
  static const TestCase TRIM_CASES[] = {
      {"", ""},
      {" ", ""},
      {"  ", ""},
      {" A", "A"},
      {"A ", "A"},
      {" A ", "A"},
      {"First", "First"},
      {"F i r s t", "F i r s t"},
      {"    F    i    r    s    t    ", "F i r s t"},
      {"F    i    r    s    t    ", "F i r s t"},
      {"    F    i    r    s    t", "F i r s t"},
  };

  for (auto test_case : TRIM_CASES) {
    const size_t result_size =
        std::max(test_case.original.length(), test_case.expected.length()) + 1;
    char result[result_size];
    result[result_size - 1] = '\0';

    strcpy(result, test_case.original.c_str());

    TrimSpaces(result, result_size);

    if (strcmp(result, test_case.expected.c_str()) != 0) {
      std::cout << "Unit test failed: Expected \"" << test_case.expected
                << "\", got \"" << result << "\"." << std::endl;
      return false;
    }
  }

  return true;
}  // namespace namearranger
}  // namespace namearranger
}  // namespace mjohnson

int main(int argc, char* argv[]) {
  bool runUnitTests;
  if (!mjohnson::common::ParseArgs(argc, argv, &runUnitTests)) {
    return 1;
  }

  if (runUnitTests) {
    const bool result = mjohnson::namearranger::RunUnitTests();

    if (!result) {
      std::cout << "Unit tests failed." << std::endl;
      return 1;
    }

    std::cout << "Unit tests passed." << std::endl;
    return 0;
  }

  mjohnson::namearranger::Run();
  return 0;
}
