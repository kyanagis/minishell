/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 05:17:23 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/05 09:24:48 by kyanagis         ###   ########.fr       */
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
}		t_shell;

t_shell	shell_init(char **envp);

void	shell_destroy(t_shell *sh);
void	free_env(char **env);

#endif