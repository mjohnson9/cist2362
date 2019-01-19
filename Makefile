BUILD_DIR ?= ${HOME}/Downloads/CIST2362
SOURCE_DIR ?= .

SRCS := $(shell find "$(SOURCE_DIR)" -iname '*.cpp' -not -name 'common.cpp' | sed 's: :\\ :g')
SRCS := $(SRCS:$(SOURCE_DIR)/%=%)
BINS := $(SRCS:%.cpp=$(BUILD_DIR)/%)

MKDIR_P := mkdir -p
RM := trash

CPPFLAGS ?= -std=c++11 -Wall


.PHONY: all clean

all: $(BINS)

$(BUILD_DIR)/common.o: $(SOURCE_DIR)/common.cpp $(SOURCE_DIR)/common.h
	-$(MKDIR_P) "$(dir $@)"
	$(COMPILE.cpp) "$<" -o "$@"

$(BUILD_DIR)/%: $(SOURCE_DIR)/%.cpp $(BUILD_DIR)/common.o
	-$(MKDIR_P) "$(dir $@)"
	$(LINK.cpp) "$<" "$(BUILD_DIR)/common.o" -o "$@"

clean:
	$(RM) -r $(BUILD_DIR)
