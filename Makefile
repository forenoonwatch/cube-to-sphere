BUILD_DIR := bin

PROJECT_DIR ?= $(CURDIR)

LIB_DIR := C:/cpplibs

CC	:= g++
CXX := g++

AR := ar

MKDIR_P ?= mkdir -p

LDFLAGS := -Wall -Wl,--major-image-version,0,--minor-image-version,0
LDLIBS := -lglu32 -lopengl32 -L"$(LIB_DIR)/lib" -lglew32 -lglfw3 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32

CPPFLAGS := -std=c++17
CXXFLAGS := -I./src -I$(LIB_DIR)/include -msse2

export

all: cubetosphere matcom

cubetosphere: common
	@$(MAKE) -C cube-to-sphere -f Makefile

#alphamap: common
#	@$(MAKE) -C alpha-map -f Makefile

matcom: common
	@$(MAKE) -C material-combiner -f Makefile

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

.PHONY: all common clean cubetosphere matcom
