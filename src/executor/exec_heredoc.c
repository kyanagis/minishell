/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:23 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 10:35:08 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_heredoc_internal.h"

int	open_heredoc(t_shell *sh, t_redir *redir)
{
	t_hd_chunk	*chunks;
	size_t		total_len;
	int			read_fd;

	if (!redir)
		return (-1);
	chunks = NULL;
	read_fd = -1;
	if (!collect_chunks(sh, redir, &chunks, &total_len))
	{
		free_chunks(chunks);
		return (-1);
	}
	if (total_len >= HEREDOC_TMP_THRESHOLD)
		read_fd = open_temp_file(chunks);
	else
		read_fd = open_pipe_reader(chunks);
	free_chunks(chunks);
	return (read_fd);
}
