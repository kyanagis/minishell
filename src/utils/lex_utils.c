/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 11:20:05 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:20:06 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	free_lexout(t_lexout *lx)
{
	size_t			n;
	char			**a;
	unsigned char	**m;

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
