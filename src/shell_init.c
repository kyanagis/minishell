/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/30 00:00:00 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <readline/history.h>

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

void	shell_init(t_shell *sh, char **envp)
{
	sh->env_list = NULL;
	sh->envp = env_dup(envp);
	sh->last_status = 0;
	sh->should_exit = false;
	sh->prompt = NULL;
	sh->table = NULL;
	if (!sh->envp)
	{
		sh->last_status = 1;
		sh->should_exit = true;
		return ;
	}
	update_prompt(sh);
}

void	shell_destroy(t_shell *sh)
{
	size_t	i;

	if (sh->envp)
	{
		i = 0;
		while (sh->envp[i])
		{
			free(sh->envp[i]);
			i++;
		}
		free(sh->envp);
	}
	if (sh->prompt)
		free(sh->prompt);
	rl_clear_history();
}
