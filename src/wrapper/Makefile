BIN_DIR                = bin
BUILD_DIR              = build

ASAN                   = y
CXX_STD                = c++17
DEBUG                  = y
LOG_LEVEL              = DEBUG
LOG_PATH               = /dev/stderr
USE_TARGET_LLVM_CONFIG = y

LLVM_HOME           = $(shell brew --prefix llvm)
CC                  = $(LLVM_HOME)/bin/clang
CXX                 = $(LLVM_HOME)/bin/clang++
CLANG_FORMAT        = $(LLVM_HOME)/bin/clang-format
TARGET_LLVM_HOME    = $(LLVM_HOME)
TARGET_CC           = $(TARGET_LLVM_HOME)/bin/clang
TARGET_CXX          = $(TARGET_LLVM_HOME)/bin/clang++
TARGET_LLVM_CONFIG  = $(TARGET_LLVM_HOME)/bin/llvm-config
ifeq ($(USE_TARGET_LLVM_CONFIG), y)
  LLVM_CONFIG       = $(TARGET_LLVM_HOME)/bin/llvm-config
else
  LLVM_CONFIG       = $(LLVM_HOME)/bin/llvm-config
endif
$(info TARGET_CC  : $(TARGET_CC))
$(info TARGET_CXX : $(TARGET_CXX))

TARGETS := base
TARGETS += $(addsuffix ++, $(TARGETS))
TARGETS +=
TARGETS := $(strip $(TARGETS))
$(info TARGETS    : $(TARGETS))
TARGETS := $(addprefix $(BIN_DIR)/, $(TARGETS))

CUSTOM_CXXFLAGS  =
CXXFLAGS        != $(LLVM_CONFIG) --cxxflags
CXXFLAGS        += -Wall -fPIC
CXXFLAGS        += -DTARGET_CC=\"$(TARGET_CC)\" -DTARGET_CXX=\"$(TARGET_CXX)\"
CXXFLAGS        += -DLOG_PATH=\"$(LOG_PATH)\" -DLOG_LEVEL=LOG_LEVEL_$(LOG_LEVEL)
LDFLAGS         != $(LLVM_CONFIG) --ldflags
LIBS            != $(LLVM_CONFIG) --libs
LDFLAGS         += $(LIBS)
ifeq ($(ASAN), y)
  CXXFLAGS      += -fsanitize=address -fno-omit-frame-pointer
  LDFLAGS       += -fsanitize=address
endif
ifneq ($(CXX_STD), )
  CXXFLAGS      := $(patsubst -std=%, -std=$(CXX_STD), $(CXXFLAGS))
endif
ifeq ($(DEBUG), y)
  CXXFLAGS      += --debug -O0
else
  CXXFLAGS      += -O3
endif
CXXFLAGS        += $(CUSTOM_CXXFLAGS)
CXXFLAGS        := $(strip $(CXXFLAGS))
LDFLAGS         := $(strip $(LDFLAGS))
$(info CXXFLAGS   : $(CXXFLAGS))
$(info LDFLAGS    : $(LDFLAGS))

all: $(TARGETS)

.PHONY: clean
clean:
	$(RM) --recursive $(BIN_DIR)
	$(RM) --recursive $(BUILD_DIR)

$(BIN_DIR) $(BUILD_DIR):
	mkdir --parents $@

COMMON_SRCS    = $(wildcard common/*.cc) wrapper/wrapper_base.cc
COMMON_HEADERS = $(COMMON_SRCS:%.cc=%.h)
COMMON_OBJS    = $(COMMON_SRCS:%.cc=%.o)

BASE_OBJS  = $(COMMON_OBJS) main.o
BASE_OBJS := $(addprefix $(BUILD_DIR)/, $(BASE_OBJS))
$(BIN_DIR)/base: $(BASE_OBJS) | $(BIN_DIR)
$(BIN_DIR)/base++: $(BASE_OBJS:%.o=%++.o) | $(BUILD_DIR)
$(BIN_DIR)/base $(BIN_DIR)/base++:
	$(info $@ <= $^)
	@ $(CXX) $(CXXFLAGS) $(LDFLAGS) --output $@ $^

$(BUILD_DIR)/%++.o: CXXFLAGS += -DUSE_CXX
$(BUILD_DIR)/%++.o: %.cc %.h | $(BUILD_DIR)
	$(info $@ <= $<)
	@ mkdir --parents $(dir $@)
	@ $(CXX) --compile $(CXXFLAGS) --output $@ $<
$(BUILD_DIR)/%.o: %.cc %.h | $(BUILD_DIR)
	$(info $@ <= $<)
	@ mkdir --parents $(dir $@)
	@ $(CXX) --compile $(CXXFLAGS) --output $@ $<
