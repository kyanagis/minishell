/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2025/01/03 00:00:00 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "executor.h"

bool	ensure_env_list(t_shell *sh)
{
	if (!sh)
		return (false);
	if (sh->env_list)
		return (true);
	sh->env_list = init_env_list(sh->envp);
	if (!sh->env_list)
		return (false);
	return (true);
}
