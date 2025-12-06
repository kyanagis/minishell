/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya < skatsuya@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 05:17:19 by sakurako          #+#    #+#             */
/*   Updated: 2025/12/07 05:04:18 by skatsuya         ###   ########.fr       */
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
# include <stdio.h>
# include <unistd.h>

typedef struct s_env
{
	char *key;          // 変数名
	char *value;        // 値　(例: "/Users/student")
	struct s_env *next; // 次の変数へのポインタ
}		t_env;

// env.c
int		ft_env(t_shell *shell);
t_env	*env_new_node(char *str);

// echo.c
int		ft_echo(char **argv);

// exit.c
int		ft_exit(char **argv, t_shell *shell);

// pwd.c
int		ft_pwd(char **args);

// export.c
void	ft_export_one(t_shell *shell, char *arg);

#endif
