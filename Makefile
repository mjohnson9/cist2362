MAKEFILE_PATH := $(lastword $(MAKEFILE_LIST))
SOURCE_DIR := $(dir $(MAKEFILE_PATH))
SOURCE_DIR := $(SOURCE_DIR:%/=%)
BUILD_DIR := $(SOURCE_DIR)/build

SRCS := $(shell find "$(SOURCE_DIR)" -iname '*.cpp' -and \( -not \( -path './common.cpp' -or -path './Template.cpp' \) \) | sort)
BINS := $(SRCS:$(SOURCE_DIR)/%.cpp=$(BUILD_DIR)/%)
TESTS := $(BINS:%=%.test)
TIDYS := $(SRCS:%=%.tidy)
LINTS := $(SRCS:%=%.lint)

MKDIR_P ?= mkdir -p
CPPLINT ?= cpplint
CLANG_TIDY ?= clang-tidy

DEBUG ?= 1

CPPFLAGS += -std=c++11 -Wall -Wextra -Wc++11-compat -Werror -pedantic-errors -ffast-math -ftrapv

ifeq ($(DEBUG), 1)
	CPPFLAGS += -glldb -O0
else
	CPPFLAGS += -Ofast -mtune=native -march=native
endif

TIDYFLAGS := $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)
TIDYFLAGS := $(TIDYFLAGS:%=-extra-arg="%")


.PHONY: all tidy lint style test clean

all: $(BINS)

$(BUILD_DIR)/common.o: $(SOURCE_DIR)/common.cpp $(SOURCE_DIR)/common.h
	@$(MKDIR_P) "$(dir $@)"
	$(COMPILE.cpp) "$<" -o "$@"

$(BUILD_DIR)/Lesson5/%: $(SOURCE_DIR)/Lesson5/%.cpp $(BUILD_DIR)/common.o
	@$(MKDIR_P) "$(dir $@)"
	$(LINK.cpp) "$(BUILD_DIR)/common.o" "$<" -DUSE_GMP -lgmpxx -lgmp -o "$@"

$(BUILD_DIR)/%: $(SOURCE_DIR)/%.cpp $(BUILD_DIR)/common.o
	@$(MKDIR_P) "$(dir $@)"
	$(LINK.cpp) "$(BUILD_DIR)/common.o" "$<" -o "$@"

$(BUILD_DIR)/%.test: $(BUILD_DIR)/%
	"$(@:%.test=%)" -test

$(SOURCE_DIR)/%.tidy: $(SOURCE_DIR)/%
	"$(CLANG_TIDY)" -header-filter="$(SOURCE_DIR)" "$(@:%.tidy=%)"

$(SOURCE_DIR)/%.lint: $(SOURCE_DIR)/%
	"$(CPPLINT)" "$(@:%.lint=%)"

tidy: $(TIDYS) $(SOURCE_DIR)/common.cpp.tidy

lint: $(LINTS) $(SOURCE_DIR)/common.cpp.lint

style: tidy lint

test: $(TESTS)
	@echo "Tests passed"

clean:
	$(RM) -r $(BUILD_DIR)
