/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:15 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 10:34:59 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "executor.h"

static size_t	env_list_len(t_env *env)
{
	size_t	len;

	len = 0;
	while (env)
	{
		++len;
		env = env->next;
	}
	return (len);
}

static void	free_env_partial(char **envp, size_t count)
{
	size_t	idx;

	if (!envp)
		return ;
	idx = 0;
	while (idx < count)
	{
		free(envp[idx]);
		++idx;
	}
	free(envp);
}

static char	*join_key_value(t_env *node)
{
	char	*tmp;
	char	*res;

	if (!node->value)
		return (ft_strdup(node->key));
	tmp = ft_strjoin(node->key, "=");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, node->value);
	free(tmp);
	return (res);
}

static bool	fill_env_entries(char **envp, t_env *cur, size_t *filled)
{
	size_t	idx;

	idx = 0;
	while (cur)
	{
		envp[idx] = join_key_value(cur);
		if (!envp[idx])
		{
			*filled = idx;
			return (false);
		}
		++idx;
		cur = cur->next;
	}
	*filled = idx;
	return (true);
}

bool	sync_envp(t_shell *sh)
{
	char	**envp;
	size_t	count;
	size_t	idx;

	if (!sh || !sh->env_list)
		return (true);
	count = env_list_len(sh->env_list);
	envp = (char **)ft_calloc(count + 1, sizeof(char *));
	if (!envp)
		return (false);
	if (!fill_env_entries(envp, sh->env_list, &idx))
	{
		free_env_partial(envp, idx);
		return (false);
	}
	envp[count] = NULL;
	free_env(sh->envp);
	sh->envp = envp;
	return (true);
}
