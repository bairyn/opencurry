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
	CFLAGS   := -std=c89 -pedantic -pedantic-errors -pthread -Wall  -Werror -g
	CPPFLAGS := -DDEBUG
else
	CFLAGS   := -std=c89 -pedantic -pedantic-errors -pthread -Wall  -Werror -O2
	CPPFLAGS := -DNDEBUG
endif

CPPFLAGS += -DNAME="$(NAME)" -DVERSION="$(VERSION)"

ALL_CFLAGS   := $(USR_CFLAGS)   $(CFLAGS)
ALL_CPPFLAGS := $(USR_CPPFLAGS) $(CPPFLAGS)

#------------------------------------------------------------------------------
# Object files.

CLI_OBJS :=                         \
	$(OBJ_DIR)/main.o                 \
	$(OBJ_DIR)/opencurry.o            \
	$(OBJ_DIR)/cli.o                  \
	$(OBJ_DIR)/lex.o                  \
	$(OBJ_DIR)/util.o                 \
	$(OBJ_DIR)/fun_prim.o             \
	$(OBJ_DIR)/fun_pair.o             \
	$(OBJ_DIR)/fun_base.o
TEST_CLI_OBJS :=                    \
	$(OBJ_DIR)/tests/main.o           \
	$(OBJ_DIR)/tests/testing.o        \
	$(OBJ_DIR)/tests/test_testing.o   \
	$(OBJ_DIR)/tests/test_all.o       \
	$(OBJ_DIR)/tests/test_opencurry.o \
	$(OBJ_DIR)/tests/test_cli.o       \
	$(OBJ_DIR)/tests/test_util.o      \
	$(OBJ_DIR)/tests/test_fun_prim.o  \
	$(OBJ_DIR)/tests/test_fun_pair.o  \
	$(OBJ_DIR)/tests/test_fun_base.o  \
	$(OBJ_DIR)/tests/test_lex.o       \
	$(OBJ_DIR)/tests/test_utf8.o

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
	$(RM) $(CLI_OBJS) $(TEST_CLI_OBJS)
	$(RM) $(CLI_BIN)  $(TEST_CLI_BIN)

.PHONY : cli test_cli
cli      : $(CLI_BIN)
test_cli : $(TEST_CLI_BIN)

$(CLI_BIN) : $(CLI_OBJS)
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) -o $@ $^

$(TEST_CLI_BIN) : $(TEST_CLI_OBJS)
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) -o $@ $^

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) -c -o $@ $<
