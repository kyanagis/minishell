/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_internal.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2025/01/03 00:00:00 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_HEREDOC_INTERNAL_H
# define EXEC_HEREDOC_INTERNAL_H

# include "executor.h"

typedef struct s_hd_chunk
{
	char				*data;
	size_t				len;
	struct s_hd_chunk	*next;
}	t_hd_chunk;

typedef struct s_chunk_state
{
	t_hd_chunk	**head;
	t_hd_chunk	**tail;
	size_t		*total_len;
}	t_chunk_state;

bool	collect_chunks(t_shell *sh, t_redir *redir,
			t_hd_chunk **head, size_t *total_len);
void	free_chunks(t_hd_chunk *head);
bool	write_chunks(int fd, t_hd_chunk *head);
int		open_temp_file(t_hd_chunk *head);
int		open_pipe_reader(t_hd_chunk *head);

#endif
