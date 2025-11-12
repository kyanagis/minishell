/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 19:59:53 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/12 09:55:11 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdlib.h>

static void	init_lexer(t_lexer *lx, size_t n)
{
	lx->work = (char *)ft_xmalloc(n * 2 + 1);
	lx->mask = (unsigned char *)ft_xmalloc(n * 2 + 1);
	lx->args = NULL;
	lx->masks = NULL;
	lx->lens = NULL;
	lx->kinds = NULL;
	lx->read_pos = 0;
	lx->write_pos = 0;
	lx->args_len = 0;
	lx->args_cap = 0;
	lx->token_start = -1;
	lx->state = LEX_GEN;
	grow_meta(lx, 1);
}

t_lexout	*tokenize(const char *line)
{
	t_lexer	lx;
	size_t	n;
	char	*input;

	if (!line)
		return (NULL);
	n = ft_strlen(line);
	input = (char *)ft_xmalloc(n + 1);
	ft_strcpy(input, line);
	init_lexer(&lx, n);
	scan_line(&lx, input);
	free(input);
	return (finalize_lexer(&lx));
}

void	lexout_free(t_lexout *o)
{
	size_t	i;

	if (!o)
		return ;
	i = 0;
	while (i < o->count)
	{
		free(o->argv[i]);
		free(o->qmask[i]);
		i++;
	}
	free(o->argv);
	free(o->qmask);
	free(o->len);
	free(o->kind);
	free(o);
}

// int	main(int argc, char **argv)
// {
// 	(void)argc;
// 	(void)argv;
// 	char *s = "ls -l |
// 	|| grep 'awd awd w $TEST $test ' << $TEST |  $TEST   '' ";
// 	// t_lexout *lx = lex_line(argv[1]);
// 	t_lexout *lx = tokenize(s);

// 	lexer_debug_print(lx);
// 	return (0);
// }