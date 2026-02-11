/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:05 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/28 18:20:01 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "built_in.h"

bool	is_builtin(const char *name)
{
	const t_builtin	*builtins;
	size_t	idx;

	if (!name)
		return (false);
	builtins = get_builtins();
	idx = 0;
	while (builtins[idx].cmd)
	{
		if (ft_strcmp(name, builtins[idx].cmd) == 0)
			return (true);
		++idx;
	}
	return (false);
}
