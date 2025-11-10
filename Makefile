NAME        := minishell
CC          := cc

BASE_CFLAGS := -Wall -Wextra -Werror

DEBUG       ?= 0
SAN         ?= 0

ifeq ($(DEBUG),1)
  CFLAGS := $(BASE_CFLAGS) -g3 -O0
else
  CFLAGS := $(BASE_CFLAGS)
endif

ifeq ($(SAN),1)
  CFLAGS += -fsanitize=address -fno-omit-frame-pointer
  LDFLAGS += -fsanitize=address -fno-omit-frame-pointer
endif

OBJ_DIR     := obj

SRC_DIRS    := src src/lexer src/parser
SRCS        := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.c))
OBJS        := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS        := $(OBJS:.o=.d)

INC_DIRS    := include libft $(SRC_DIRS)
CPPFLAGS    := $(addprefix -I,$(INC_DIRS)) -MMD -MP

LIBFT_DIR   := libft
LIBFT_A     := $(LIBFT_DIR)/libft.a

# ===== OS 判定 =====
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)                # macOS
  RL_DIR := $(shell brew --prefix readline 2>/dev/null)
  ifneq ($(RL_DIR),)
    RL_INC := -I$(RL_DIR)/include
    RL_LIB := -L$(RL_DIR)/lib -lreadline
  else
    RL_INC :=
    RL_LIB := -lreadline
  endif
else                                    # Linux想定
  RL_HAS_PKG := $(shell pkg-config --exists readline && echo yes || echo no)
  ifeq ($(RL_HAS_PKG),yes)
    RL_INC := $(shell pkg-config --cflags readline)
    RL_LIB := $(shell pkg-config --libs readline)
  else
    RL_INC :=
    RL_LIB := -lreadline
  endif
endif

CPPFLAGS += $(RL_INC)

.PHONY: all clean fclean re bonus norm print run

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) $(RL_LIB) $(LDFLAGS) -o $@

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

bonus:
	$(MAKE) -C $(LIBFT_DIR) bonus
	$(MAKE) $(NAME)

norm:
	norminette $(SRC_DIRS) $(INC_DIRS) $(LIBFT_DIR)

print:
	@echo "UNAME_S  = $(UNAME_S)"
	@echo "NAME     = $(NAME)"
	@echo "SRC_DIRS = $(SRC_DIRS)"
	@echo "SRCS     = $(SRCS)"
	@echo "OBJS     = $(OBJS)"
	@echo "INC_DIRS = $(INC_DIRS)"
	@echo "RL_INC   = $(RL_INC)"
	@echo "RL_LIB   = $(RL_LIB)"
	@echo "CFLAGS   = $(CFLAGS)"
	@echo "LDFLAGS  = $(LDFLAGS)"

run: $(NAME)
	./$(NAME)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(RM) $(NAME)

re: fclean all

-include $(DEPS)
