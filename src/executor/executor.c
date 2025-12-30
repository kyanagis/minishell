/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:50 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 10:35:24 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	dispatch_execute(t_shell *sh, t_pipeline *pl)
{
	if (!pl || pl->ncmds == 0)
		return (0);
	if (pl->ncmds == 1)
		return (execute_single(sh, pl->cmds[0]));
	return (execute_pipeline(sh, pl));
}

int	execute_commands(t_shell *sh, t_pipeline *pl)
{
	int	status;

	if (!sh)
		return (STATUS_GENERAL_ERR);
	if (!ensure_env_list(sh))
		return (STATUS_GENERAL_ERR);
	status = dispatch_execute(sh, pl);
	sh->last_status = status;
	return (status);
}
