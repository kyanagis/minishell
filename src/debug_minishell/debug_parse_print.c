
#include "lexer.h"
#include "parser.h"
#include <stdio.h>

static void print_indent(size_t depth)
{
	size_t i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		++i;
	}
}

static const char *redir_kind_str(t_redir_kind kind)
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

static int has_any_quote(const unsigned char *mask, size_t len)
{
	size_t i;

	if (!mask)
		return (0);
	i = 0;
	while (i < len)
	{
		if (mask[i] != 0)
			return (1);
		++i;
	}
	return (0);
}
static void print_token_quote_info(const t_lexout *lx, size_t tok_i,
								   size_t depth)
{
	size_t j;
	const char *kind_s;
	unsigned char *mask;

	if (!lx || tok_i >= lx->count)
	{
		print_indent(depth);
		printf("token: (no lx / out of range)\n");
		return;
	}
	kind_s = token_str(lx->kind[tok_i]);
	print_indent(depth);
	printf("token[%zu]: kind=%s len=%zu argv=\"%s\"",
		   tok_i, kind_s, lx->len[tok_i],
		   (lx->argv[tok_i] != NULL) ? lx->argv[tok_i] : "(null)");
	printf(" has_quote=%s\n",
		   has_any_quote(lx->qmask[tok_i], lx->len[tok_i]) ? "true" : "false");
	print_indent(depth);
	printf("qmask: ");
	mask = lx->qmask[tok_i];
	if (!mask)
	{
		printf("(null)\n");
		return;
	}
	j = 0;
	while (j < lx->len[tok_i])
	{
		printf("%u", (unsigned int)mask[j]);
		if (j + 1 < lx->len[tok_i])
			printf(" ");
		++j;
	}
	printf("\n");
}

static void print_argv(const t_cmd *cmd, const t_lexout *lx, size_t depth)
{
	size_t i;

	if (!cmd->argv || cmd->argc == 0 || !cmd->argv[0])
	{
		print_indent(depth);
		printf("argv: (empty)\n");
		return;
	}
	print_indent(depth);
	printf("argc = %zu\n", cmd->argc);
	i = 0;
	while (i < cmd->argc && cmd->argv[i])
	{
		print_indent(depth);
		printf("argv[%zu]: \"%s\"", i,
			   (cmd->argv[i] != NULL) ? cmd->argv[i] : "(null)");
		if (cmd->tok_idx_argv)
		{
			printf(" (tok=%zu)", cmd->tok_idx_argv[i]);
			printf("\n");
			if (lx)
				print_token_quote_info(lx, cmd->tok_idx_argv[i], depth + 1);
		}
		else
		{
			printf(" (tok=none)\n");
		}
		++i;
	}
}

static void print_redirs(const t_redir *r, const t_lexout *lx, size_t depth)
{
	if (!r)
	{
		print_indent(depth);
		printf("redirs: (none)\n");
		return;
	}
	while (r)
	{
		print_indent(depth);
		printf("redir @%p:\n", (void *)r);
		print_indent(depth + 1);
		printf("kind=%s, arg=\"%s\"\n",
			   redir_kind_str(r->kind),
			   (r->arg != NULL) ? r->arg : "(null)");
		print_indent(depth + 1);
		printf("delim_quoted=%s, tok_idx=%zu, next=%p\n",
			   r->delim_quoted ? "true" : "false",
			   r->tok_idx,
			   (void *)r->next);
		if (lx)
			print_token_quote_info(lx, r->tok_idx, depth + 1);
		r = r->next;
	}
}

static void print_cmd(const t_cmd *cmd, size_t index, const t_lexout *lx)
{
	print_indent(1);
	printf("cmd[%zu] @%p:\n", index, (void *)cmd);
	if (!cmd)
	{
		print_indent(2);
		printf("(null)\n");
		return;
	}
	print_argv(cmd, lx, 2);
	print_redirs(cmd->redirs, lx, 2);
}

void debug_print_pipeline(const t_pipeline *pl, const t_lexout *lx)
{
	size_t i;

	if (!pl)
	{
		printf("[pipeline NULL]\n");
		return;
	}
	printf("========== PIPELINE DUMP ==========\n");
	printf("pipeline @%p\n", (void *)pl);
	printf("ncmds = %zu\n", pl->ncmds);
	i = 0;
	while (i < pl->ncmds)
	{
		print_cmd(pl->cmds[i], i, lx);
		++i;
	}
	printf("============ END DUMP ============\n");
}
