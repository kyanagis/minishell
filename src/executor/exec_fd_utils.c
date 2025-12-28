/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fd_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:20 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 10:35:04 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "executor.h"

void init_fd_target(t_fd_target *tgt, int in_fd, int out_fd)
{
	if (!tgt)
		return;
	tgt->in_fd = in_fd;
	tgt->out_fd = out_fd;
}

int apply_fd_target(t_fd_target *tgt)
{
	if (!tgt)
		return (STATUS_GENERAL_ERR);
	if (tgt->in_fd != STDIN_FILENO && dup2(tgt->in_fd, STDIN_FILENO) < 0)
		return (STATUS_GENERAL_ERR);
	if (tgt->out_fd != STDOUT_FILENO && dup2(tgt->out_fd, STDOUT_FILENO) < 0)
		return (STATUS_GENERAL_ERR);
	return (0);
}

void close_fd_target(t_fd_target *tgt)
{
	if (!tgt)
		return;
	if (tgt->in_fd >= 0 && tgt->in_fd != STDIN_FILENO)
		close(tgt->in_fd);
	if (tgt->out_fd >= 0 && tgt->out_fd != STDOUT_FILENO)
		close(tgt->out_fd);
	init_fd_target(tgt, STDIN_FILENO, STDOUT_FILENO);
}
