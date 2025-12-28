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

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "executor.h"
#include "expander.h"

typedef struct s_hd_chunk
{
	char				*data;
	size_t				len;
	struct s_hd_chunk	*next;
}	t_hd_chunk;

static void	free_chunks(t_hd_chunk *head)
{
	t_hd_chunk	*next;

	while (head)
	{
		next = head->next;
		free(head->data);
		free(head);
		head = next;
	}
}

static bool	append_chunk(t_hd_chunk **head, t_hd_chunk **tail,
		char *line, size_t len)
{
	t_hd_chunk	*node;

	node = (t_hd_chunk *)ft_calloc(1, sizeof(t_hd_chunk));
	if (!node)
		return (false);
	node->data = line;
	node->len = len;
	if (*tail)
		(*tail)->next = node;
	else
		*head = node;
	*tail = node;
	return (true);
}

static char	*join_line_with_newline(char *line, size_t *out_len)
{
	size_t	len;
	char	*res;

	len = ft_strlen(line);
	res = (char *)ft_calloc(len + 2, sizeof(char));
	if (!res)
		return (NULL);
	ft_memcpy(res, line, len);
	res[len] = '\n';
	res[len + 1] = '\0';
	*out_len = len + 1;
	return (res);
}

static char	*expand_heredoc_line(t_shell *sh, char *line, bool quoted)
{
	t_expand_input	input;
	unsigned char	*mask;
	char			*expanded;

	if (quoted)
		return (line);
	mask = (unsigned char *)ft_calloc(ft_strlen(line), sizeof(unsigned char));
	if (!mask)
	{
		free(line);
		return (NULL);
	}
	input.src = line;
	input.mask = mask;
	input.len = ft_strlen(line);
	if (!expand_word(sh, &input, &expanded))
	{
		free(mask);
		free(line);
		return (NULL);
	}
	free(mask);
	free(line);
	return (expanded);
}

static bool	collect_chunks(t_shell *sh, t_redir *redir,
		t_hd_chunk **head, size_t *total_len)
{
	char		*line;
	char		*expanded;
	char		*with_nl;
	t_hd_chunk	*tail;
	size_t		chunk_len;

	*total_len = 0;
	tail = NULL;
	line = readline(HEREDOC_PROMPT);
	while (line)
	{
		if (ft_strcmp(line, redir->arg) == 0)
		{
			free(line);
			return (true);
		}
		expanded = expand_heredoc_line(sh, line, redir->delim_quoted);
		if (!expanded)
			return (false);
		with_nl = join_line_with_newline(expanded, &chunk_len);
		free(expanded);
		if (!with_nl || !append_chunk(head, &tail, with_nl, chunk_len))
		{
			free(with_nl);
			return (false);
		}
		*total_len += chunk_len;
		line = readline(HEREDOC_PROMPT);
	}
	return (true);
}

static bool	write_chunks(int fd, t_hd_chunk *head)
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

static bool	read_urandom(unsigned char *buf, size_t len)
{
	int		fd;
	size_t	read_total;
	ssize_t	rc;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (false);
	read_total = 0;
	while (read_total < len)
	{
		rc = read(fd, buf + read_total, len - read_total);
		if (rc <= 0)
		{
			close(fd);
			return (false);
		}
		read_total += (size_t)rc;
	}
	close(fd);
	return (true);
}

static char	*build_temp_path(const unsigned char *bytes, size_t len)
{
	const char	*prefix;
	size_t		i;
	char		*path;
	const char	*base;
	char		*hex;

	prefix = ".heredoc_";
	base = "0123456789abcdef";
	hex = (char *)ft_calloc(len * 2 + 1, sizeof(char));
	if (!hex)
		return (NULL);
	i = 0;
	while (i < len)
	{
		hex[i * 2] = base[(bytes[i] >> 4) & 0x0F];
		hex[i * 2 + 1] = base[bytes[i] & 0x0F];
		++i;
	}
	path = ft_strjoin(prefix, hex);
	free(hex);
	return (path);
}

static int	open_temp_file(t_hd_chunk *head)
{
	unsigned char	bytes[HEREDOC_RAND_SIZE];
	char			*path;
	int				fd;
	int				read_fd;
	int				tries;

	tries = 0;
	while (tries < 16)
	{
		if (!read_urandom(bytes, HEREDOC_RAND_SIZE))
			return (-1);
		path = build_temp_path(bytes, HEREDOC_RAND_SIZE);
		if (!path)
			return (-1);
		fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0600);
		if (fd >= 0)
			break ;
		free(path);
		++tries;
	}
	if (fd < 0)
		return (-1);
	if (!write_chunks(fd, head))
	{
		close(fd);
		unlink(path);
		free(path);
		return (-1);
	}
	close(fd);
	read_fd = open(path, O_RDONLY);
	unlink(path);
	free(path);
	return (read_fd);
}

static int	open_pipe_reader(t_hd_chunk *head)
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
