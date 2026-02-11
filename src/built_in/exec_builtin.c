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

static const t_builtin	g_builtins[] = {
	{"cd", ft_cd},
	{"echo", ft_echo},
	{"pwd", ft_pwd},
	{"export", ft_export},
	{"unset", ft_unset},
	{"env", ft_env},
	{"exit", ft_exit},
	{NULL, NULL}
};

const t_builtin	*get_builtins(void)
{
	return (g_builtins);
}

int	exec_builtin(t_shell *shell, char **argv)
{
	const t_builtin	*builtins;
	int				i;

	if (!argv || !argv[0])
		return (NOT_BUILTIN);
	builtins = get_builtins();
	i = 0;
	while (builtins[i].cmd)
	{
		if (ft_strcmp(argv[0], builtins[i].cmd) == 0)
			return (builtins[i].func(shell, argv));
		i++;
	}
	return (NOT_BUILTIN);
}
