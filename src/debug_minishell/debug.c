#include "debug.h"
#include <stdio.h>

void	redir_list_destroy(t_redir **head)
{
	t_redir	*cur;
	t_redir	*next;

	if (!head || !*head)
		return ;
	cur = *head;
	while (cur)
	{
		next = cur->next;
		if (cur->arg)
			free(cur->arg);
		free(cur);
		cur = next;
	}
	*head = NULL;
}

void	cmd_destroy(t_cmd **pcmd)
{
	size_t	i;
	t_cmd	*cmd;

	if (!pcmd || !*pcmd)
		return ;
	cmd = *pcmd;
	if (cmd->argv)
	{
		i = 0;
		while (i < cmd->argc)
		{
			free(cmd->argv[i]);
			++i;
		}
		free(cmd->argv);
	}
	if (cmd->tok_idx_argv)
		free(cmd->tok_idx_argv);
	if (cmd->redirs)
		redir_list_destroy(&cmd->redirs);
	free(cmd);
	*pcmd = NULL;
}
void	pipeline_destroy(t_pipeline **ppl)
{
	size_t		i;
	t_pipeline	*pl;

	if (!ppl || !*ppl)
		return ;
	pl = *ppl;
	if (pl->cmds)
	{
		i = 0;
		while (i < pl->ncmds)
		{
			if (pl->cmds[i])
				cmd_destroy(&pl->cmds[i]);
			++i;
		}
		free(pl->cmds);
	}
	free(pl);
	*ppl = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_shell		sh;
	t_pipeline	*pl;
	t_lexout	*tokens;

	pl = NULL;
	shell_init(&sh, envp);
	// debug_minishell_readline(&sh);
	// (void)argv;
	(void)argc;
	(void)pl;
	// (void)sh;
	(void)envp;
	// char *s = argv[1];
	// scanf("%hhd", s);
	tokens = tokenize(argv[1]);
	lexer_debug_print(tokens);
#ifndef PARSE
	parse_tokens(&sh, tokens, &pl);
#else
	build_pipeline_from_tokens(tokens, &pl);
#endif
	debug_print_pipeline(pl, tokens);
	free_lexout(tokens);
	pipeline_destroy(&pl);
	shell_destroy(&sh);
	return (0);
}
