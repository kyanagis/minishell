#include "debug.h"

static void	print_debug_argv(const t_cmd *cmd, size_t depth)
{
	size_t	idx;

	if (!cmd || !cmd->argv)
		return ;
	idx = 0;
	while (idx < cmd->argc && cmd->argv[idx])
	{
		printf("%*sargv[%zu] = \"%s\"\n", (int)depth, "", idx, cmd->argv[idx]);
		++idx;
	}
}

static void	print_debug_redirs(const t_redir *redir, size_t depth)
{
	while (redir)
	{
		printf("%*sredir kind=%d arg=\"%s\"\n", (int)depth, "",
			redir->kind, redir->arg);
		redir = redir->next;
	}
}

void	debug_expand_pipeline(const t_pipeline *pl)
{
	size_t	idx;

	if (!pl)
	{
		printf("[debug_expand] pipeline is NULL\n");
		return ;
	}
	printf("===== debug_expand: %zu commands =====\n", pl->ncmds);
	idx = 0;
	while (idx < pl->ncmds)
	{
		printf("-- cmd[%zu] --\n", idx);
		print_debug_argv(pl->cmds[idx], 2);
		print_debug_redirs(pl->cmds[idx]->redirs, 2);
		++idx;
	}
	printf("===== end debug_expand =====\n");
}
