#------------------------------------------------------------------------------
# Settings.

NAME      := opencurry
VERSION   := 0.1.0

BUILD_DIR := build
OBJ_DIR   := $(BUILD_DIR)/obj
SRC_DIR   := src

#------------------------------------------------------------------------------
# Flags.

USR_CFLAGS   :=
USR_CPPFLAGS :=

ifeq ($(DEBUG),1)
	CFLAGS   := -std=c89 -pedantic -pedantic-errors -pthreads -Wall  -Werr -g
	CPPFLAGS := -DDEBUG
else
	CFLAGS   := -std=c89 -pedantic -pedantic-errors -pthreads -Wall  -Werr -O2
	CPPFLAGS := -DNDEBUG
endif

CPPFLAGS += -DNAME="$(NAME)" -DVERSION="$(VERSION)"

ALL_CFLAGS   := $(USR_CFLAGS)   $(CFLAGS)
ALL_CPPFLAGS := $(USR_CPPFLAGS) $(CPPFLAGS)

#------------------------------------------------------------------------------
# Object files.

CLI_OBJS := \
	$(OBJ_DIR)/main.o \
	$(OBJ_DIR)/opencurry.o \
	$(OBJ_DIR)/cli.o
TEST_CLI_OBJS := \
	$(OBJ_DIR)/tests/main.o \
	$(OBJ_DIR)/tests/test_all.o \
	$(OBJ_DIR)/tests/test_opencurry.o \
	$(OBJ_DIR)/tests/test_cli.o

#------------------------------------------------------------------------------
# Build files.

CLI_BIN      := $(BUILD_DIR)/$(NAME)
TEST_CLI_BIN := $(BUILD_DIR)/test-$(NAME)

#------------------------------------------------------------------------------
# Targets.

.PHONY : all
all : cli test_cli


.PHONY : clean
clean :
	$(RM) $(CLI_OBJS) $(TEST_CLI_OBJS) $(BUILD)

.PHONY : cli test_cli
cli      : $(CLI_BIN)
test_cli : $(TEST_CLI_BIN)

$(CLI_BIN) : $(CLI_OBJS)
	$(CC) -o $@ $(CLI_OBJS)

$(TEST_CLI_BIN) : $(TEST_CLI_OBJS)
	$(CC) -o $@ $(TEST_CLI_OBJS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) -c -o $@ $<
