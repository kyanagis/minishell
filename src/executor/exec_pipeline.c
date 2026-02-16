/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:30 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/16 23:29:32 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

ssize_t	run_pipeline(t_shell *sh, t_pipeline *pl, pid_t *pids);

int	execute_pipeline(t_shell *sh, t_pipeline *pl)
{
	pid_t	*pids;
	ssize_t	launched;
	int		status;

	if (!pl || pl->ncmds == 0)
		return (0);
	pids = (pid_t *)ft_calloc(pl->ncmds, sizeof (pid_t));
	if (!pids)
		return (STATUS_GENERAL_ERR);
	launched = run_pipeline(sh, pl, pids);
	status = 0;
	if (launched > 0)
		status = wait_children(pids, launched);
	free(pids);
	if (launched < 0)
		return (STATUS_GENERAL_ERR);
	return (status);
}
