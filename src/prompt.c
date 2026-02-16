/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/16 23:23:57 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_prompt(t_shell *sh)
{
	if (sh->prompt)
		free(sh->prompt);
	sh->last_status %= 256;
	sh->prompt = ft_strdup("minishell$ ");
}

// sh->prompt = _test_build_prompt(sh);

// static char	*_test_build_prompt(t_shell *sh)
// {
// 	char	*status;
// 	char	*tmp;
// 	char	*res;

// 	status = ft_itoa(sh->last_status);
// 	if (!status)
// 		return (ft_strdup("minishell "));
// 	tmp = ft_strjoin("minishell:", status);
// 	free(status);
// 	if (!tmp)
// 		return (ft_strdup("minishell> "));
// 	res = ft_strjoin(tmp, "> ");
// 	free(tmp);
// 	if (!res)
// 		return (ft_strdup("minishell$ "));
// 	return (res);
// }