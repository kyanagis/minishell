#include "debug.h"
#include <stdio.h>

void redir_list_destroy(t_redir **head)
{
	t_redir *cur;
	t_redir *next;

	if (!head || !*head)
		return;
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

void cmd_destroy(t_cmd **pcmd)
{
	size_t i;
	t_cmd *cmd;

	if (!pcmd || !*pcmd)
		return;
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
void pipeline_destroy(t_pipeline **ppl)
{
	size_t i;
	t_pipeline *pl;

	if (!ppl || !*ppl)
		return;
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

static void run_debug_pipeline(t_shell *sh, const char *line)
{
	t_pipeline *pl;
	t_lexout *tokens;
	bool ok;

	pl = NULL;
	tokens = tokenize(line);
	if (!tokens)
		return;
	lexer_debug_print(tokens);
#ifndef PARSE
	ok = parse_tokens(sh, tokens, &pl);
#else
	ok = build_pipeline_from_tokens(tokens, &pl);
#endif
	if (!ok)
	{
		free_lexout(tokens);
		return;
	}
	debug_print_pipeline(pl, tokens);
	expand_pipeline(sh, tokens, pl);
	debug_expand_pipeline(pl);
	free_lexout(tokens);
	pipeline_destroy(&pl);
}

static char *read_debug_line(void)
{
	char *line;
	size_t cap;
	ssize_t len;

	line = NULL;
	cap = 0;
	len = getline(&line, &cap, stdin);
	if (len < 0)
	{
		free(line);
		return (NULL);
	}
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

int main(int argc, char **argv, char **envp)
{
	t_shell sh;
	char *line;

	(void)argc;
	(void)argv;
	shell_init(&sh, envp);
	while (1)
	{
		write(STDOUT_FILENO, "debug> ", 7);
		line = read_debug_line();
		if (!line)
			break;
		if (*line == '\0')
		{
			free(line);
			continue;
		}
		run_debug_pipeline(&sh, line);
		free(line);
	}
	shell_destroy(&sh);
	return (0);
}
