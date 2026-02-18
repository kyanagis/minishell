/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 05:17:23 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/16 23:25:56 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// #include <readline/history.h>
// #include <readline/readline.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>
# include <stdio.h>
# include "libft.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum e_tok_kind
{
	TOK_WORD,
	TOK_PIPE,
	TOK_LT,
	TOK_GT,
	TOK_DLT,
	TOK_DGT
}	t_tok_kind;

typedef struct s_lexout
{
	char			**argv;
	unsigned char	**qmask;
	size_t			*len;
	t_tok_kind		*kind;
	size_t			count;
}	t_lexout;

typedef enum e_redir_kind
{
	R_IN,
	R_OUT,
	R_HEREDOC,
	R_APPEND
}	t_redir_kind;

typedef struct s_redir
{
	t_redir_kind	kind;
	char			*arg;
	bool			delim_quoted;
	size_t			tok_idx;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char	**argv;
	size_t	argc;
	size_t	*tok_idx_argv;
	t_redir	*redirs;
}	t_cmd;

typedef struct s_pipeline
{
	size_t	ncmds;
	t_cmd	**cmds;
}	t_pipeline;

typedef struct s_free_table	t_free_table;

typedef struct s_shell
{
	int				last_status;
	char			**envp;
	char			*prompt;
	char			*current_line;
	t_env			*env_list;
	bool			should_exit;
	t_free_table	*table;
}	t_shell;

void		shell_init(t_shell *sh, char **envp);
void		update_prompt(t_shell *sh);
void		shell_destroy(t_shell *sh);
void		free_env(char **env);
const char	*get_envp_value_len(t_shell *sh, const char *key, size_t len);
const char	*get_envp_value(t_shell *sh, const char *key);
char		*read_non_tty_line_fd(int fd);

void		parse_and_execute(t_shell *sh, t_free_table *table,
				t_lexout *tokens);
bool		is_eof(char *line);
bool		should_skip_line(t_shell *sh, char *line);

void		ft_envlst_delone(t_env *lst, void (*del)(void *));
void		free_env_list(t_env **lst, void (*del)(void *));
t_env		*env_new_node(char *str);
void		env_add_back(t_env **head, t_env *new_node);
t_env		*init_env_list(char **envp);

#endif
