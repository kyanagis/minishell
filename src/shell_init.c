/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 08:50:39 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/05 09:19:49 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

size_t	env_count(char **p)
{
	char	**q;

	if (!p)
		return (0);
	q = p;
	while (*q)
		q++;
	return ((size_t)(q - p));
}

static void	free_env_until(char **head, char **end)
{
	while (head < end)
	{
		free(*head);
		head++;
	}
}

char	**env_dup(char **src)
{
	size_t	n;
	char	**dst;
	char	**d;
	char	**s;

	n = env_count(src);
	dst = (char **)ft_xmalloc(sizeof(char *) * (n + 1));
	d = dst;
	s = src;
	while (s && *s)
	{
		*d = ft_strdup(*s);
		if (!*d)
		{
			free_env_until(dst, d);
			free(dst);
			return (NULL);
		}
		d++;
		s++;
	}
	*d = NULL;
	return (dst);
}

t_shell	shell_init(char **envp)
{
	t_shell	sh;

	sh.envp = env_dup(envp);
	sh.last_status = 0;
	return (sh);
}
