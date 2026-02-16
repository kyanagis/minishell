/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_write.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/16 23:30:20 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_heredoc_internal.h"

bool	write_chunks(int fd, t_hd_chunk *head)
{
	size_t	written;
	ssize_t	rc;

	while (head)
	{
		written = 0;
		while (written < head->len)
		{
			rc = write(fd, head->data + written, head->len - written);
			if (rc < 0)
				return (false);
			written += (size_t)rc;
		}
		head = head->next;
	}
	return (true);
}

int	open_pipe_reader(t_hd_chunk *head)
{
	int	fds[2];

	if (pipe(fds) == -1)
		return (-1);
	if (!write_chunks(fds[1], head))
	{
		close(fds[0]);
		close(fds[1]);
		return (-1);
	}
	close(fds[1]);
	return (fds[0]);
}
