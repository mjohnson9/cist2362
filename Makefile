MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
SOURCE_DIR ?= $(dir $(MAKEFILE_PATH))
BUILD_DIR ?= $(SOURCE_DIR)/build

SRCS := $(shell find "$(SOURCE_DIR)" -iname '*.cpp' -not -name 'common.cpp' | sed 's: :\\ :g')
SRCS := $(SRCS:$(SOURCE_DIR)/%=%)
BINS := $(SRCS:%.cpp=$(BUILD_DIR)/%)
TESTS := $(BINS:%=%.test)
TIDYS := $(SRCS:%=$(SOURCE_DIR)/%.tidy)

MKDIR_P := mkdir -p
RM := trash

CPPFLAGS ?= -std=c++11 -Wall -O0

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
	clang-tidy $(TIDYFLAGS) "$(@:%.tidy=%)"

tidy: $(TIDYS) $(SOURCE_DIR)/common.cpp.tidy

test: $(TESTS)
	@echo "Tests passed"

clean:
	$(RM) -r $(BUILD_DIR)
