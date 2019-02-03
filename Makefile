MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
SOURCE_DIR := $(dir $(MAKEFILE_PATH))
SOURCE_DIR := $(SOURCE_DIR:%/=%)
BUILD_DIR := $(SOURCE_DIR)/build

SRCS := $(shell find "$(SOURCE_DIR)" -iname '*.cpp' -not -name 'common.cpp' | sort)
BINS := $(SRCS:$(SOURCE_DIR)/%.cpp=$(BUILD_DIR)/%)
TESTS := $(BINS:%=%.test)
TIDYS := $(SRCS:%=%.tidy)
LINTS := $(SRCS:%=%.lint)

MKDIR_P ?= mkdir -p
CPPLINT ?= cpplint
CLANG_TIDY ?= clang-tidy

CPPFLAGS ?= -std=c++11 -Wall -Wconversion -Wextra -Wc++11-compat -Werror -pedantic-errors -ffast-math -O0 -g

TIDYFLAGS := $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)
TIDYFLAGS := $(TIDYFLAGS:%=-extra-arg="%")


.PHONY: all test tidy clean

all: $(BINS)

$(BUILD_DIR)/common.o: $(SOURCE_DIR)/common.cpp $(SOURCE_DIR)/common.h
	@$(MKDIR_P) "$(dir $@)"
	$(COMPILE.cpp) "$<" -o "$@"

$(BUILD_DIR)/%: $(SOURCE_DIR)/%.cpp $(BUILD_DIR)/common.o
	@$(MKDIR_P) "$(dir $@)"
	$(LINK.cpp) "$<" "$(BUILD_DIR)/common.o" -o "$@"

$(BUILD_DIR)/%.test: $(BUILD_DIR)/%
	"$(@:%.test=%)" -test

$(SOURCE_DIR)/%.tidy: $(SOURCE_DIR)/%
	"$(CLANG_TIDY)" -header-filter="$(SOURCE_DIR)" $(TIDYFLAGS) "$(@:%.tidy=%)"

$(SOURCE_DIR)/%.lint: $(SOURCE_DIR)/%
	"$(CPPLINT)" "$(@:%.lint=%)"

tidy: $(TIDYS) $(SOURCE_DIR)/common.cpp.tidy

lint: $(LINTS) $(SOURCE_DIR)/common.cpp.lint

test: $(TESTS)
	@echo "Tests passed"

clean:
	$(RM) -r $(BUILD_DIR)
