/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya < skatsuya@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 05:17:19 by sakurako          #+#    #+#             */
/*   Updated: 2026/01/20 20:12:37 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

# define TRUE 1
# define FALSE 0
# define ERROR 1
# define NO_ERROR 0
# define NOT_BUILTIN -1
# define EXIT_NUMERIC_STATUS 255

/* message */
# define MSG_EXIT_TOO_MANY_ARGS "minishell: exit: too many arguments\n"

# include "minishell.h"

/* env.c */
int						ft_env(t_shell *shell, char **argv);

/* echo.c */
int						ft_echo(t_shell *shell, char **argv);

/* exit.c */
int						ft_exit(t_shell *shell, char **argv);

/* export_util.c */
void					ft_export_one(t_shell *shell, char *arg);
bool					append_env_copy(t_env **copy_head, t_env *env_node);
t_env					*copy_env_list(t_env *env_list);
void					sort_env_list(t_env *head);
void					swap_node(t_env *node_a, t_env *node_b);

/* export.c */
int						ft_export(t_shell *shell, char **argv);

/* pwd.c */
int						ft_pwd(t_shell *shell, char **argv);

/* unset.c */
int						ft_unset(t_shell *shell, char **argv);

/* cd.c */
int						ft_cd(t_shell *shell, char **argv);

/* exec_builtin.c */
int						exec_builtin(t_shell *shell, char **argv);

typedef struct s_builtin
{
	char				*cmd;
	int					(*func)(t_shell *shell, char **argv);
}						t_builtin;

const t_builtin			*get_builtins(void);

#endif
