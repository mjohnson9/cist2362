#include <algorithm>
#include <iostream>
#include <string>

namespace wordcount {
	// Runs unit tests to confirm program functionality
	bool RunUnitTests();
	// Gets an entire line of input from a user. Returns a string to avoid limitations of buffer size.
	std::string GetInputString();
	// Prompts the user to ask if they would like to continue. Re-prompts on
	// invalid input. Once valid input is received, it returns the result.
	bool RequestContinue();
	int CountWords(const std::string);
	int CountWords(const char*);
	bool IsLetter(const char);

	void Run() {
		if (RunUnitTests()) {
			std::cout << "Unit tests passed." << std::endl;
		}
		else {
			std::cout << "Unit tests failed." << std::endl;
			return;
		}

		do {
			std::cout << "Enter a sentence or phrase to have its words counted:" << std::endl;
			const std::string input = GetInputString();
			const int words = CountWords(input);
			std::cout << "\"" << input << "\" has " << words << " words in it." << std::endl;
		} while (RequestContinue());
	}

	int CountWords(const std::string str) {
		return CountWords(str.c_str());
	}

	int CountWords(const char* str) {
		if (str[0] == '\0') {
			// This is a zero-length string; shortcut
			return 0;
		}

		// Initialize based on first character
		int words = (IsLetter(str[0]) ? 1 : 0);
		bool lastWasWord = IsLetter(str[0]);

		for (size_t i = 1; str[i] != '\0'; i++) { // Skip the first character; we already processed it
			const char c = str[i];
			if (!IsLetter(c)) {
				lastWasWord = false;
				continue;
				// This is a non-word; don't count words
			}

			if (lastWasWord) {
				continue;
				// This is a word following a word; don't count a new word
			}

			// This is a word following a non-word; count a word
			words += 1;
			lastWasWord = true; // Don't count the next characters as words
		}

		return words;
	}

	bool IsLetter(const char c) {
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	std::string GetInputString() {
		std::string response;
		{ // Put this in its own scope to reduce scope pollution
			char next_char;
			do {
				next_char = std::cin.peek();
				if (next_char == '\r' || next_char == '\n') {
					std::cin.get(next_char);  // Remove the newline from the buffer
				}
			} while (next_char == '\r' || next_char == '\n');  // Continue as long
															   // as we encounter
															   // newlines
		}
		std::getline(std::cin, response);
		return response;
	}

	bool RequestContinue() {
		while (true) {
			std::cout << "Would you like to run the program again? ";

			std::string response = GetInputString();
			if (response.empty()) {
				return false;  // The default option is to not continue
			}

			// Lowercase the response to make it easier to compare
			std::transform(response.begin(), response.end(), response.begin(),
				::tolower);
			if (response == "y" || response == "yes") {
				return true;
			}
			if (response == "n" || response == "no") {
				return false;
			}

			std::cout << response
				<< " is an invalid response. Available responses are yes, y, "
				"no, or n."
				<< std::endl
				<< std::endl;
		}
	}

	bool RunUnitTests() {
		const static size_t NUM_CASES = 23;
		const static std::string CASES[NUM_CASES] = {
			"", " ", "  ", // Spaces aren't words; expect 0
			"0", " 0", "0 ", " 0 ", // Numbers aren't words; expect 0
			"_", " _", "_ ", " _ ", // Special characters aren't words; expect 0
			"Test", "  Test", "Test  ", "  Test  ", // One word
			"Test  case", "  Test case", "Test  case  ", "  Test  case  ", // Two words
			"T e s t", " T e s t", "T e s t ", " T e s t " // Four "words"
		};
		const static size_t EXPECTED_COUNT[NUM_CASES] = {
			0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 1, 1,
			2, 2, 2, 2,
			4, 4, 4, 4
		};

		for (size_t i = 0; i < NUM_CASES; i++) {
			const std::string test_case = CASES[i];
			const size_t case_expected_count = EXPECTED_COUNT[i];
			const int words = CountWords(test_case);
			if (words != case_expected_count) {
				std::cout << "Failed on unit test " << (i + 1) << ": Expected " << case_expected_count << " words, got " << words << std::endl;
				return false;
			}
		}

		return true;
	}
}

int main() {
	wordcount::Run();
}