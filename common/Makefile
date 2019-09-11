PROJECT_NAME := Common

BUILD_DIR := $(PROJECT_DIR)/bin
SRC_DIR := src

TARGET := lib$(PROJECT_NAME).a

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d)) 

SRCS := $(call rwildcard, $(SRC_DIR)/, *.cpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	@$(AR) rcs $@ $(OBJS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	@echo "Building $(notdir $@)..."
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: all
