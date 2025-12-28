/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 19:59:41 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/29 07:54:52 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <unistd.h>

static void putn(const char *s, size_t n)
{
	write(1, s, n);
}

static void putu(unsigned long v)
{
	char buf[32];
	int i;

	i = 31;
	buf[i--] = '\n';
	if (v == 0)
	{
		buf[i--] = '0';
		write(1, &buf[i + 1], 2);
		return;
	}
	while (v && i >= 0)
	{
		buf[i--] = '0' + (v % 10);
		v /= 10;
	}
	write(1, &buf[i + 1], 31 - i);
}

static void print_kind(t_tok_kind kind)
{
	if (kind == TOK_WORD)
		putn("WORD  : ", 8);
	else if (kind == TOK_PIPE)
		putn("PIPE  : ", 8);
	else if (kind == TOK_LT)
		putn("LT    : ", 8);
	else if (kind == TOK_GT)
		putn("GT    : ", 8);
	else if (kind == TOK_DLT)
		putn("DLT   : ", 8);
	else
		putn("DGT   : ", 8);
}

static void print_qmask(const unsigned char *mask, size_t len)
{
	size_t i;
	char c;

	putn("\nqmask : ", 9);
	i = 0;
	while (i < len)
	{
		c = (char)(mask[i] + '0');
		write(1, &c, 1);
		i++;
	}
}

void lexer_debug_print(const t_lexout *o)
{
	size_t i;

	if (!o)
	{
		write(1, "(lex error)\n", 12);
		return;
	}
	i = 0;
	while (i < o->count)
	{
		print_kind(o->kind[i]);
		putn(o->argv[i], o->len[i]);
		print_qmask(o->qmask[i], o->len[i]);
		putn("\nlen   : ", 9);
		putu((unsigned long)o->len[i]);
		putn("\n\n", 2);
		i++;
	}
}
