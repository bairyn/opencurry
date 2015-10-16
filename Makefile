#------------------------------------------------------------------------------
# Settings.

NAME             := opencurry
VERSION          := 0.1.0

BUILD_DIR        := build
OBJ_DIR          := $(BUILD_DIR)/obj
SRC_DIR          := src

RES_DIR          := resource
TEST_RES_DIR     := $(RES_DIR)/tests

RES_OBJ_DIR      := $(BUILD_DIR)/resource
TEST_RES_OBJ_DIR := $(BUILD_DIR)/$(RES_DIR)/tests

MKDIR_P          := mkdir -p

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
# Directories.

BUILD_DIRECTORIES :=                \
	$(BUILD_DIR)                      \
	$(OBJ_DIR)                        \
	$(OBJ_DIR)/tests                  \
	$(RES_OBJ_DIR)                    \
	$(TEST_RES_OBJ_DIR)

#------------------------------------------------------------------------------
# Object files.

SHARED_OBJS :=                      \
	$(OBJ_DIR)/resources.o            \
	$(OBJ_DIR)/opencurry.o            \
	$(OBJ_DIR)/cli.o                  \
	$(OBJ_DIR)/lex.o                  \
	$(OBJ_DIR)/util.o                 \
	$(OBJ_DIR)/fun_prim.o             \
	$(OBJ_DIR)/fun_pair.o             \
	$(OBJ_DIR)/fun_base.o

CLI_OBJS :=                         \
	$(SHARED_OBJS)                    \
	$(OBJ_DIR)/main.o
TEST_CLI_OBJS :=                    \
	$(SHARED_OBJS)                    \
	$(OBJ_DIR)/tests/main.o           \
	$(OBJ_DIR)/tests/testing.o        \
	$(OBJ_DIR)/tests/resources.o      \
	$(OBJ_DIR)/tests/test_testing.o   \
	$(OBJ_DIR)/tests/test_all.o       \
	$(OBJ_DIR)/tests/test_resources.o \
	$(OBJ_DIR)/tests/test_opencurry.o \
	$(OBJ_DIR)/tests/test_cli.o       \
	$(OBJ_DIR)/tests/test_util.o      \
	$(OBJ_DIR)/tests/test_fun_prim.o  \
	$(OBJ_DIR)/tests/test_fun_pair.o  \
	$(OBJ_DIR)/tests/test_fun_base.o  \
	$(OBJ_DIR)/tests/test_lex.o       \
	$(OBJ_DIR)/tests/test_utf8.o

#------------------------------------------------------------------------------
# Resource files, to be statically linked with the binary executable.

CLI_RESOURCE_OBJS :=                           \
	$(RES_OBJ_DIR)/tmp.txt.o
TEST_CLI_RESOURCE_OBJS :=                      \
	$(CLI_RESOURCE_OBJS)                         \
	$(TEST_RES_OBJ_DIR)/static_string_test.txt.o

#------------------------------------------------------------------------------
# Build files.

CLI_BIN      := $(BUILD_DIR)/$(NAME)
TEST_CLI_BIN := $(BUILD_DIR)/test-$(NAME)


#------------------------------------------------------------------------------
# Main targets.

# Build everything: compiler and test suites.
.PHONY : all
all : build-directories cli test_cli


# Remove all build files.
.PHONY : clean clean-executables clean-code-objs clean-res-objs
clean : clean-executables clean-code-objs clean-res-objs
clean-executables :
	# Remove application binaries.
	$(RM) $(CLI_BIN)  $(TEST_CLI_BIN)
clean-code-objs :
	# Remove application code object files.
	$(RM) $(CLI_OBJS) $(TEST_CLI_OBJS)
clean-res-objs :
	# Remove application code object files.
	$(RM) $(CLI_RESOURCE_OBJS) $(TEST_CLI_RESOURCE_OBJS)

# Remove all build files and directories.
.PHONY : cleanall clean-build-directories
cleanall : clean clean-build-directories
clean-build-directories :
	# Remove all build directories.
	$(RM) -r $(BUILD_DIRECTORIES)


# Create all build directories.
.PHONY : build-directories
build-directories : $(BUILD_DIRECTORIES)

$(BUILD_DIRECTORIES) :
	$(MKDIR_P) $@

#------------------------------------------------------------------------------
# Executable targets.

.PHONY : cli test_cli
cli      :  $(CLI_BIN) | build-directories
test_cli :  $(TEST_CLI_BIN) | build-directories

$(CLI_BIN) : $(CLI_OBJS) $(CLI_RESOURCE_OBJS) | build-directories
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) -o $@ $^

$(TEST_CLI_BIN) : $(TEST_CLI_OBJS) $(TEST_CLI_RESOURCE_OBJS) | build-directories
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) -o $@ $^

#------------------------------------------------------------------------------
# Application code.

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | build-directories
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) -c -o $@ $<

#------------------------------------------------------------------------------
# Resource objects.

# http://stackoverflow.com/a/4158997

$(RES_OBJ_DIR)/%.o : $(RES_DIR)/%.res | build-directories
	$(LD) -r -b binary -o $@ $<
