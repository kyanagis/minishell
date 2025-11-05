#とりあえずのメイクファイルなので修正します
NAME        := minishell

CC          := cc
CFLAGS      ?= -Wall -Wextra -Werror
DEBUG       ?= 1          # 1: -g3 -O0, 0: -O2
SAN         ?= 0          # 1: AddressSanitizer (開発用)

ifeq ($(DEBUG),1)
  CFLAGS += -g3 -O0
else
  CFLAGS += -O2
endif
ifeq ($(SAN),1)
  CFLAGS  += -fsanitize=address -fno-omit-frame-pointer
  LDFLAGS += -fsanitize=address
endif

OBJ_DIR     := obj

SRC_DIRS    := src src/lexer src/parser
SRCS        := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.c))
OBJS        := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS        := $(OBJS:.o=.d)

INC_DIRS    := include libft $(SRC_DIRS)
CPPFLAGS    := $(addprefix -I,$(INC_DIRS)) -MMD -MP

RL_DIR      := $(shell brew --prefix readline 2>/dev/null)
ifeq ($(RL_DIR),)
  RL_HAS_PKG := $(shell pkg-config --exists readline && echo yes || echo no)
  ifeq ($(RL_HAS_PKG),yes)
    RL_INC  := $(shell pkg-config --cflags readline)
    RL_LIB  := $(shell pkg-config --libs   readline)
  else
    RL_INC  :=
    RL_LIB  := -lreadline
  endif
else
  RL_INC    := -I$(RL_DIR)/include
  RL_LIB    := -L$(RL_DIR)/lib -lreadline
endif
CPPFLAGS    += $(RL_INC)

LIBFT_DIR   := libft
LIBFT_A     := $(LIBFT_DIR)/libft.a

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
	@echo "NAME     = $(NAME)"
	@echo "SRC_DIRS = $(SRC_DIRS)"
	@echo "SRCS     = $(SRCS)"
	@echo "OBJS     = $(OBJS)"
	@echo "INC_DIRS = $(INC_DIRS)"
	@echo "RL_LIB   = $(RL_LIB)"
	@echo "CFLAGS   = $(CFLAGS)"

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
