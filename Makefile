#TARGET_EXEC := CubeToSphere

BUILD_DIR := bin
#SRC_DIR := src

PROJECT_DIR ?= $(CURDIR)

LIB_DIR := C:/cpplibs

CC	:= g++
CXX := g++

AR := ar

MKDIR_P ?= mkdir -p

LDFLAGS := -Wall -Wl,--major-image-version,0,--minor-image-version,0
LDLIBS := -lglu32 -lopengl32 -L"$(LIB_DIR)/Glew/lib" -lglew32 -L"$(LIB_DIR)/GLFW/lib" -lglfw3 -L"$(LIB_DIR)/GLM/lib" -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32

CPPFLAGS := -std=c++17
CXXFLAGS := -I./src -I$(LIB_DIR)/Glew/include -I$(LIB_DIR)/GLFW/include -I$(LIB_DIR)/GLM/include -msse2

export

all: cubetosphere

#$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
#	@echo "Building $(TARGET_EXEC)..."
#	@$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

cubetosphere: common
	@$(MAKE) -C cube-to-sphere -f Makefile

common:
	@$(MAKE) -C common -f Makefile

clean:
	@echo "Cleaning object files..."
	@cd $(BUILD_DIR) && $(RM) -rd *
	@cd $(BUILD_DIR) && $(RM) -rf *.o
	@echo "Cleaning archive files..."
	@cd $(BUILD_DIR) && $(RM) -rf *.a
	@echo "Cleaning executable files..."
	@cd $(BUILD_DIR) && $(RM) -rf *.exe

.PHONY: all common clean
