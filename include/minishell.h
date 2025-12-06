/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya < skatsuya@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 05:17:23 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/06 23:12:54 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <signal.h>

#include <readline/history.h>
#include <readline/readline.h>
#include "libft.h"
#include "lexer.h"
extern volatile sig_atomic_t g_sig;

typedef struct s_shell
{
	int last_status; //$?
	char **envp;	 // execve用
	char *prompt;
	t_env *env_list;
} t_shell;

void shell_init(t_shell *sh, char **envp);

void update_prompt(t_shell *sh);
void shell_destroy(t_shell *sh);
void free_env(char **env);

#endif
