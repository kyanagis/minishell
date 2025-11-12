/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_parse_print.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 05:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/12 17:19:42 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdio.h>

static void	print_indent(size_t depth)
{
	size_t	i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		++i;
	}
}

static const char	*redir_kind_str(t_redir_kind kind)
{
	if (kind == R_IN)
		return ("<");
	if (kind == R_OUT)
		return (">");
	if (kind == R_HEREDOC)
		return ("<<");
	if (kind == R_APPEND)
		return (">>");
	return ("?");
}

static void	print_redirs(const t_redir *r, size_t depth)
{
	if (!r)
	{
		print_indent(depth);
		printf("redirs: (none)\n");
		return ;
	}
	while (r)
	{
		print_indent(depth);
		printf("redir: kind=%s, arg=\"%s\"", redir_kind_str(r->kind),
			(r->arg != NULL) ? r->arg : "(null)");
		if (r->kind == R_HEREDOC && r->delim_quoted)
			printf(" [delim_quoted]");
		printf("\n");
		r = r->next;
	}
}

static void	print_argv(char *const *argv, size_t depth)
{
	size_t	i;

	if (!argv || !argv[0])
	{
		print_indent(depth);
		printf("argv: (empty)\n");
		return ;
	}
	print_indent(depth);
	printf("argv:");
	i = 0;
	while (argv[i])
	{
		printf(" [%s]", argv[i]);
		++i;
	}
	printf("\n");
}

static void	print_cmd(const t_cmd *cmd, size_t index)
{
	print_indent(1);
	printf("cmd[%zu]:\n", index);
	if (!cmd)
	{
		print_indent(2);
		printf("(null)\n");
		return ;
	}
	print_argv(cmd->argv, 2);
	print_redirs(cmd->redirs, 2);
}

void	debug_print_pipeline(const t_pipeline *pl)
{
	size_t	i;

	if (!pl)
	{
		printf("[pipeline NULL]\n");
		return ;
	}
	printf("========== PIPELINE DUMP ==========\n");
	printf("ncmds = %zu\n", pl->ncmds);
	i = 0;
	while (i < pl->ncmds)
	{
		print_cmd(pl->cmds[i], i);
		++i;
	}
	printf("============ END DUMP ============\n");
}
