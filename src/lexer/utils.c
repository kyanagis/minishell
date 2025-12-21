/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 20:00:03 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/15 04:05:33 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdlib.h>
#include <unistd.h>

int	is_dq_escapable(char c)
{
	return (c == '"' || c == '\\' || c == '$' || c == '`');
}

int	lx_isblank(int c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

void	cleanup_lexer(t_lexer *lx)
{
	free(lx->args);
	free(lx->masks);
	free(lx->lens);
	free(lx->kinds);
	free(lx->work);
	free(lx->mask);
}
