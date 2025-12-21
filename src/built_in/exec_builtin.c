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

int exec_builtin(t_shell *shell, char **argv)
{
	int i;
	if (!argv || !argv[0])
		return (NOT_BUILTIN);
	i = 0;
	while (g_builtins[i].cmd)
	{
		if (ft_strcmp(argv[0], g_builtins[i].cmd) == 0)
			return (g_builtins[i].func(shell, argv));
		i++;
	}
	return (NOT_BUILTIN);
}
