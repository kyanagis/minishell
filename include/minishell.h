/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakurako <sakurako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 05:17:23 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/05 03:17:38 by sakurako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>

#include <readline/history.h>
#include <readline/readline.h>
#include "libft.h"
#include "lexer.h"
#include "sig.h"

typedef struct s_shell
{
	int last_status; //$?
	char **envp;	 // execve用
	char *prompt;
} t_shell;

void shell_init(t_shell *sh, char **envp);

void update_prompt(t_shell *sh);
void shell_destroy(t_shell *sh);
void free_env(char **env);

#endif