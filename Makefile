NAME        := minishell
CC          := cc

BASE_CFLAGS := -Wall -Wextra -Werror

DEBUG       ?= 0
SAN         ?= 0
AFL         ?= 0

ifneq (,$(findstring 1,$(AFL)))
  CC := afl-gcc
endif

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

SRC_DIRS    := src src/lexer src/parser src/parser/parse_builders \
               src/parser/parse_core src/parser/parse_syntax \
               src/parser/parse_tokens src/expand \
               src/built_in \
               src/executor \
               src/utils \
               src/utils/free

SRCS        := \
               src/main.c \
               src/main_utils.c \
               src/prompt.c \
               src/shell_init.c \
               src/signals.c \
               src/built_in/cd.c \
               src/built_in/echo.c \
               src/built_in/env.c \
               src/built_in/env_utils.c \
               src/built_in/exec_builtin.c \
               src/built_in/exit.c \
               src/built_in/export.c \
               src/built_in/export_util.c \
               src/built_in/pwd.c \
               src/built_in/unset.c \
               src/executor/exec_builtin_check.c \
               src/executor/exec_child.c \
               src/executor/exec_cleanup.c \
               src/executor/exec_cmd.c \
               src/executor/exec_env.c \
               src/executor/exec_env_init.c \
               src/executor/exec_error.c \
               src/executor/exec_fd_utils.c \
               src/executor/exec_heredoc.c \
               src/executor/exec_heredoc_chunks.c \
               src/executor/exec_heredoc_chunks_process.c \
               src/executor/exec_heredoc_chunks_utils.c \
               src/executor/exec_heredoc_tempfile.c \
               src/executor/exec_heredoc_write.c \
               src/executor/exec_path.c \
               src/executor/exec_pipeline.c \
               src/executor/exec_pipeline_runner.c \
               src/executor/exec_redir.c \
               src/executor/exec_single.c \
               src/executor/exec_wait.c \
               src/executor/executor.c \
               src/expand/expand_buffer.c \
               src/expand/expand_cleanup.c \
               src/expand/expand_env.c \
               src/expand/expand_word.c \
               src/expand/expander.c \
               src/lexer/finalize.c \
               src/lexer/grow.c \
               src/lexer/handle_gen.c \
               src/lexer/handle_quote.c \
               src/lexer/lexer.c \
               src/lexer/lexer_cleanup.c \
               src/lexer/push.c \
               src/lexer/scan.c \
               src/lexer/utils.c \
               src/parser/parse_builders/command_builder.c \
               src/parser/parse_builders/pipeline_builder.c \
               src/parser/parse_core/parse_cleanup.c \
               src/parser/parse_core/parse_context.c \
               src/parser/parse_core/parse_finalize.c \
               src/parser/parse_core/parse_utils.c \
               src/parser/parse_core/parser.c \
               src/parser/parse_core/pipeline_parser.c \
               src/parser/parse_syntax/redir_syntax_scan.c \
               src/parser/parse_tokens/token_dispatcher.c \
               src/parser/parse_tokens/token_pipe_handler.c \
               src/parser/parse_tokens/token_redir_handler.c \
               src/parser/parse_tokens/token_word_handler.c \
               src/utils/env_utils.c \
               src/utils/lex_utils.c \
               src/utils/free/free_destroy.c \
               src/utils/free/free_table.c
OBJS        := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS        := $(OBJS:.o=.d)

INC_DIRS    := include libft $(SRC_DIRS)
CPPFLAGS    := $(addprefix -I,$(INC_DIRS)) -MMD -MP

LIBFT_DIR   := libft
LIBFT_A     := $(LIBFT_DIR)/libft.a

ifeq ($(PARSE),1)
  CPPFLAGS += -DPARSE
endif

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
  RL_DIR := $(shell brew --prefix readline 2>/dev/null)
  ifneq ($(RL_DIR),)
    RL_INC := -I$(RL_DIR)/include
    RL_LIB := -L$(RL_DIR)/lib -lreadline
  else
    RL_INC :=
    RL_LIB := -lreadline
  endif
else                                    # Linux
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

.PHONY: all clean fclean re bonus norm print run

-include $(DEPS)
