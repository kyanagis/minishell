#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef enum e_redir_kind
{
	R_IN,
	R_OUT,
	R_HEREDOC,
	R_APPEND
}					t_redir_kind;

typedef struct s_redir
{
	t_redir_kind	kind;
	char			*arg;
	bool			delim_quoted;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
}					t_cmd;

typedef struct s_pipeline
{
	size_t			ncmds;
	t_cmd			**cmds;
}					t_pipeline;

typedef struct s_cmd_b
{
	char			**argv;
	size_t			argc;
	size_t			cap;
	t_redir			*r_head;
	t_redir			*r_tail;
}					t_cmd_b;

typedef struct s_pl_b
{
	t_cmd			**arr;
	size_t			len;
	size_t			cap;
}					t_pl_b;

typedef struct s_ctx
{
	const t_lexout	*lx;
	t_pl_b			plb;
	t_cmd_b *cur;           // 現在構築中のコマンド（NULLならまだ無い）
	bool need_redir_arg;    // リダイレクト直後（次は word 必須）
	t_redir_kind pend_kind; // そのリダイレクト種別
}					t_ctx;

typedef struct s_work
{
	size_t			idx;
	t_tok_kind		kind;
	bool			ok;
	bool			quoted;
}					t_work;

typedef bool		(*t_parse_handler)(t_lexout *lx, size_t i);
bool				cmd_b_is_completely_empty(const t_cmd_b *b);
void				finalize_pipeline(t_ctx *ctx, t_work *work);

t_cmd				*cmd_b_finalize(t_cmd_b *b);
t_pipeline			*pl_b_finalize(t_pl_b *pb);
bool				parse_tokens(t_shell *sh, t_lexout *tokens,
						t_pipeline **pl);
bool				parse_syntax_error(const char *unexpected);
const char			*token_str(t_tok_kind kind);

bool				dgt_syntax_scan(t_lexout *tokens, size_t i);
bool				dlt_syntax_scan(t_lexout *tokens, size_t i);
bool				gt_syntax_scan(t_lexout *tokens, size_t i);
bool				lt_syntax_scan(t_lexout *tokens, size_t i);
// bool				parse_handle_pipe(t_ctx *ctx, t_work *work);
void				pl_b_push(t_pl_b *pb, t_cmd *c);

bool				parse_handle_word(t_ctx *ctx, t_work *work,
						const t_lexout *lx);
bool				parse_handle_redir_op(t_ctx *ctx, t_work *work);
bool				parse_handle_pipe(t_ctx *ctx, t_work *work);
bool				parse_build_pipeline(const t_lexout *lx, t_pipeline **out);
void				handle_operator(t_work *work, t_ctx *ctx,
						const t_lexout *lx);

#endif