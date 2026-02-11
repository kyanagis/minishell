/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 11:20:00 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:20:01 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

void	free_env(char **env)
{
	char	**p;

	if (!env)
		return ;
	p = env;
	while (*p)
	{
		free(*p);
		p++;
	}
	free(env);
}

const char	*get_envp_value_len(t_shell *sh, const char *key, size_t len)
{
	size_t	idx;

	if (!sh || !sh->envp || !key || len == 0)
		return (NULL);
	idx = 0;
	while (sh->envp[idx])
	{
		if (ft_strncmp(sh->envp[idx], key, len) == 0
			&& sh->envp[idx][len] == '=')
			return (sh->envp[idx] + len + 1);
		++idx;
	}
	return (NULL);
}

const char	*get_envp_value(t_shell *sh, const char *key)
{
	if (!key)
		return (NULL);
	return (get_envp_value_len(sh, key, ft_strlen(key)));
}
