/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya <skatsuya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 16:51:53 by skatsuya          #+#    #+#             */
/*   Updated: 2025/12/13 15:17:45 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

t_builtin_fn	get_builtin_func(const char *name)
{
	if (!name)
		return (NULL);
	if (ft_strcmp(name, "cd") == 0)
		return (ft_cd);
	if (ft_strcmp(name, "echo") == 0)
		return (ft_echo);
	if (ft_strcmp(name, "pwd") == 0)
		return (ft_pwd);
	if (ft_strcmp(name, "export") == 0)
		return (ft_export);
	if (ft_strcmp(name, "unset") == 0)
		return (ft_unset);
	if (ft_strcmp(name, "env") == 0)
		return (ft_env);
	if (ft_strcmp(name, "exit") == 0)
		return (ft_exit);
	return (NULL);
}

int	exec_builtin(t_shell *shell, char **argv)
{
	t_builtin_fn	func;

	if (!argv || !argv[0])
		return (NOT_BUILTIN);
	func = get_builtin_func(argv[0]);
	if (!func)
		return (NOT_BUILTIN);
	return (func(shell, argv));
}
