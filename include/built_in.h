/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya < skatsuya@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 05:17:19 by sakurako          #+#    #+#             */
/*   Updated: 2026/01/11 06:39:49 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
#define BUILT_IN_H

#define TRUE 1
#define FALSE 0
#define ERROR 1
#define NO_ERROR 0
#define NOT_BUILTIN -1
#define EXIT_NUMERIC_STATUS 255

// message
#define MSG_EXIT_TOO_MANY_ARGS "minishell: exit: too many arguments\n"

#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

// env.c
int ft_env(t_shell *shell, char **argv);

// env_utils.c
void ft_envlst_delone(t_env *lst, void (*del)(void *));
void free_env_list(t_env **lst, void (*del)(void *));

// echo.c
int ft_echo(t_shell *shell, char **argv);

// exit.c
int ft_exit(t_shell *shell, char **argv);

// export_util.c
void ft_export_one(t_shell *shell, char *arg);
void env_add_back(t_env **head, t_env *new_node);

// export.c
int ft_export(t_shell *shell, char **argv);

// pwd.c
int ft_pwd(t_shell *shell, char **argv);

// unset.c
int ft_unset(t_shell *shell, char **argv);

// cd.c
int ft_cd(t_shell *shell, char **argv);

t_env *init_env_list(char **envp);

typedef struct s_builtin
{
	char *cmd;
	int (*func)(t_shell *shell, char **argv);
} t_builtin;

static const t_builtin g_builtins[] = {
	{"cd", ft_cd},
	{"echo", ft_echo},
	{"pwd", ft_pwd},
	{"export", ft_export},
	{"unset", ft_unset},
	{"env", ft_env},
	{"exit", ft_exit},
	{NULL, NULL}};

#endif
