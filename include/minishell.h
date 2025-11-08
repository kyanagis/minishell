/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 05:17:23 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/08 09:10:55 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "lexer.h"
# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>

// volatile sig_atomic_t	g_sig;

typedef struct s_shell
{
	int last_status; //$?
	char **envp;     // execve用
	char	*prompt;
}			t_shell;

void		shell_init(t_shell *sh, char **envp);

void		shell_destroy(t_shell *sh);
void		free_env(char **env);

#endif