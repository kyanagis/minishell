/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya <skatsuya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 05:17:19 by sakurako          #+#    #+#             */
/*   Updated: 2025/12/12 12:04:19 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

# define TRUE 1
# define FALSE 0
# define ERROR 1
# define NO_ERROR 0
# define EXIT_NUMERIC_STATUS 255

// message
# define MSG_EXIT_TOO_MANY_ARGS "minishell: exit: too many arguments\n"

# include "libft.h"
# include "minishell.h"
# include <stdio.h>
# include <unistd.h>
# include <errno.h>

typedef struct s_env
{
	char *key;          // 変数名
	char *value;        // 値　(例: "/Users/student")
	struct s_env *next; // 次の変数へのポインタ
}		t_env;

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
	{NULL, NULL}
};

// env.c
int		ft_env(t_shell *shell, char **argv);
t_env	*env_new_node(char *str);

// echo.c
int		ft_echo(t_shell *shell, char **argv);

// exit.c
int		ft_exit(char **argv, t_shell *shell);

// export_util.c
void	ft_export_one(t_shell *shell, char *arg);

// export.c
int ft_export(t_shell *shell, char **argv);

// pwd.c
int ft_pwd(t_shell *shell, char **argv);

// unset.c
int ft_unset(t_shell *shell, char **argv);

// cd.c
int ft_cd(t_shell *shell, char **argv);

#endif
