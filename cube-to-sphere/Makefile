PROJECT_NAME := CubeToSphere

BUILD_DIR := $(PROJECT_DIR)/bin
SRC_DIR := src

TARGET := $(PROJECT_NAME).exe

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d)) 

SRCS := $(call rwildcard, $(SRC_DIR)/, *.cpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

CXXFLAGS := -I$(PROJECT_DIR)/common/src $(CXXFLAGS)
LDLIBS := -L"$(BUILD_DIR)" -lCommon $(LDLIBS)

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	@$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	@echo "Building $(notdir $@)..."
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: all
