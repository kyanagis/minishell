/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 08:50:55 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/05 09:24:37 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_destroy(t_shell *sh)
{
	if (!sh)
		return ;
	free_env(sh->envp);
}

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
void	free_lexout(t_lexout *lx)
{
	size_t n;
	char **a;
	unsigned char **m;

	if (!lx)
		return ;
	n = lx->count;
	a = lx->argv;
	m = lx->qmask;
	while (n--)
	{
		free(*a);
		free(*m);
		a++;
		m++;
	}
	free(lx->argv);
	free(lx->qmask);
	free(lx->len);
	free(lx->kind);
	free(lx);
}