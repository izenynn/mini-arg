# **************************************************************************** #
#                                   PROGRAM                                    #
# **************************************************************************** #

NAME = libmarg.a

# **************************************************************************** #
#                                     VARS                                     #
# **************************************************************************** #

UNAME_S := $(shell uname -s)

MAKE = make

CP = cp
MV = mv
MKDIR = mkdir -p
RM = rm -rf

MAX_OPTIONS ?= 100

# **************************************************************************** #
#                                   COMPILER                                   #
# **************************************************************************** #

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Wshadow
CFLAGS += -D MAX_OPTIONS=$(MAX_OPTIONS)

AR = ar
ARFLAGS = rcs

# **************************************************************************** #
#                                     DIRS                                     #
# **************************************************************************** #

SRC_DIR = src
OBJ_DIR = build
INC_DIR = include

# **************************************************************************** #
#                                   SOURCES                                    #
# **************************************************************************** #

SRC_FILES = \
	marg.c			\
	marg_utils.c	\

OBJ_FILES = $(SRC_FILES:%.c=%.o)

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJ = $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))
DEP = $(OBJ:%.o=%.d)

# **************************************************************************** #
#                                    FLAGS                                     #
# **************************************************************************** #

CFLAGS += -I./$(INC_DIR)

# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

PHONY := all
all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

PHONY += sanitize
ifeq ($(UNAME_S),Linux)
sanitize: CFLAGS += -g3 -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=bounds -fsanitize=null
endif
ifeq ($(UNAME_S),Darwin)
sanitize: CFLAGS += -g3 -fsanitize=address
endif
sanitize: $(NAME)

PHONY += thread
thread: CFLAGS += -g3 -fsanitize=thread
thread: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	$(MKDIR) $(OBJ_DIR)

PHONY += clean
clean:
	$(RM) $(OBJ_DIR)

PHONY += fclean
fclean: clean
	$(RM) $(NAME)

PHONY += re
re: fclean all

-include $(DEP)

.PHONY: $(PHONY)
