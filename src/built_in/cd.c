/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 07:55:29 by skatsuya          #+#    #+#             */
/*   Updated: 2026/02/16 23:17:04 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

void update_oldpwd(t_shell *shell);
static void update_pwd(t_shell *shell);
static char *get_env(t_shell *shell, char *key);
static int print_cd_error(char *dest, int err_num);

int ft_cd(t_shell *shell, char **argv)
{
	char *dest;

	if (argv[1] && argv[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (ERROR);
	}
	if (argv[1])
		dest = argv[1];
	else
		dest = get_env(shell, "HOME");
	if (!dest)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		return (ERROR);
	}
	if ((chdir(dest)) < 0)
		return (print_cd_error(dest, errno));
	update_oldpwd(shell);
	update_pwd(shell);
	return (NO_ERROR);
}

// 現在のPWD環境変数の値を検索し、OLDPWDとして保存
void update_oldpwd(t_shell *shell)
{
	t_env *current;
	char *new_arg;

	current = shell->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, "PWD") == 0)
		{
			if (!current->value)
				return;
			new_arg = ft_strjoin("OLDPWD=", current->value);
			if (!new_arg)
				return;
			ft_export_one(shell, new_arg);
			free(new_arg);
			return;
		}
		current = current->next;
	}
}

static void update_pwd(t_shell *shell)
{
	char *cwd;
	char *new_arg;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return;
	}
	new_arg = ft_strjoin("PWD=", cwd);
	if (!new_arg)
	{
		free(cwd);
		return;
	}
	ft_export_one(shell, new_arg);
	free(new_arg);
	free(cwd);
}

static char *get_env(t_shell *shell, char *key)
{
	t_env *current;

	current = shell->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static int print_cd_error(char *dest, int err_num)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(dest, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(err_num), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (ERROR);
}
