/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_tempfile.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2025/01/03 00:00:00 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "exec_heredoc_internal.h"

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

static int	open_unique_temp(char **path)
{
	unsigned char	bytes[HEREDOC_RAND_SIZE];
	int				fd;
	int				tries;

	tries = 0;
	while (tries < 16)
	{
		if (!read_urandom(bytes, HEREDOC_RAND_SIZE))
			return (-1);
		*path = build_temp_path(bytes, HEREDOC_RAND_SIZE);
		if (!*path)
			return (-1);
		fd = open(*path, O_WRONLY | O_CREAT | O_EXCL, 0600);
		if (fd >= 0)
			return (fd);
		free(*path);
		*path = NULL;
		++tries;
	}
	return (-1);
}

static int	open_read_fd(const char *path, int fd, t_hd_chunk *head)
{
	int	read_fd;

	if (!write_chunks(fd, head))
	{
		close(fd);
		unlink(path);
		return (-1);
	}
	close(fd);
	read_fd = open(path, O_RDONLY);
	if (read_fd < 0)
		unlink(path);
	return (read_fd);
}

int	open_temp_file(t_hd_chunk *head)
{
	char	*path;
	int		fd;
	int		read_fd;

	path = NULL;
	fd = open_unique_temp(&path);
	if (fd < 0)
		return (-1);
	read_fd = open_read_fd(path, fd, head);
	if (read_fd < 0)
	{
		free(path);
		return (-1);
	}
	unlink(path);
	free(path);
	return (read_fd);
}
