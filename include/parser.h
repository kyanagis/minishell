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

typedef bool		(*t_parse_handler)(t_lexout *lx, size_t i);

void				parse_tokens(t_lexout *tokens);

#endif