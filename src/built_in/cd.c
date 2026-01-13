/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya < skatsuya@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 07:55:29 by skatsuya          #+#    #+#             */
/*   Updated: 2026/01/13 19:49:48 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "minishell.h"

void		update_oldpwd(t_shell *shell);
static void	update_pwd(t_shell *shell);
static char	*get_env(t_shell *shell, char *key);

int	ft_cd(t_shell *shell, char **argv)
{
	char	*dest;
	int		error_temp;

	if (argv[1])
		dest = argv[1];
	else
		dest = get_env(shell, "HOME");
	if (!dest)
	{
		ft_putstr_fd("bash: cd: HOME not set\n", STDERR_FILENO);
		return (ERROR);
	}
	if ((chdir(dest)) < 0)
	{
		error_temp = errno;
		ft_putstr_fd("bash: cd: ", STDERR_FILENO);
		ft_putstr_fd(dest, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(error_temp), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (ERROR);
	}
	update_oldpwd(shell);
	update_pwd(shell);
	return (NO_ERROR);
}

void	update_oldpwd(t_shell *shell)
{
	t_env	*current;
	char	*new_arg;

	current = shell->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, "PWD") == 0)
		{
			new_arg = ft_strjoin("OLDPWD=", current->value);
			ft_export_one(shell, new_arg);
			free(new_arg);
			return ;
		}
		current = current->next;
	}
}

static void	update_pwd(t_shell *shell)
{
	char	*cwd;
	char	*new_arg;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return ;
	}
	new_arg = ft_strjoin("PWD=", cwd);
	ft_export_one(shell, new_arg);
	free(new_arg);
	free(cwd);
}

static char	*get_env(t_shell *shell, char *key)
{
	t_env	*current;

	current = shell->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
