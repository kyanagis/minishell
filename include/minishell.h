/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 05:17:23 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:33:43 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "libft.h"
# include "lexer.h"
# include "sig.h"

extern volatile sig_atomic_t	g_sig;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

struct							s_free_table;

typedef struct s_shell
{
	int					last_status;
	char				**envp;
	char				*prompt;
	t_env				*env_list;
	bool				should_exit;
	struct s_free_table	*table;
}	t_shell;

void		shell_init(t_shell *sh, char **envp);
void		update_prompt(t_shell *sh);
void		shell_destroy(t_shell *sh);
void		free_env(char **env);
const char	*get_envp_value_len(t_shell *sh, const char *key, size_t len);
const char	*get_envp_value(t_shell *sh, const char *key);

#endif
