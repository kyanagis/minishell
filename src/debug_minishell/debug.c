#include "debug.h"

// static void	debug_minishell(t_shell *sh)
// {
// 	char		*line;
// 	t_lexout	*tokens;
// 	ssize_t		tmp;

// 	while (1)
// 	{
// 		update_prompt(sh); // これ多分いらない
// 		sh->last_status = 0;
// 		line = readline(sh->prompt);
// 		if (!line)
// 		{
// 			tmp = write(1, "exit\n", 5);
// 			(void)tmp;
// 			break ;
// 		}
// 		if (!*line) // debug用
// 		{
// 			free(line);
// 			// continue ;
// 			break ;
// 		}
// 		if (g_sig == SIGINT)
// 		{ // Ctrl-C 中断（プロンプト再表示）
// 			sh->last_status = 130;
// 			g_sig = 0;
// 			free(line);
// 			continue ;
// 		}
// 		add_history(line);
// 		tokens = tokenize(line);
// 		free(line);
// 		if (!tokens)
// 		{
// 			sh->last_status = 1;
// 			continue ;
// 		}
// 		if (!parse_tokens(sh, tokens))
// 		{
// 			free_lexout(tokens);
// 			continue ;
// 		}
// 		// else
// 		// 	sh->last_status = 0;
// 		// ここで実行:
// 		// sh->last_status = execute(sh, tokens);
// 		free_lexout(tokens);
// 	}
// 	shell_destroy(sh);
// 	return (sh->last_status);
// }

static void free_argv(char **argv)
{
	size_t i;

	if (!argv)
		return;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		++i;
	}
	free(argv);
}

static void redir_clear(t_redir **head)
{
	t_redir *cur;
	t_redir *next;

	if (!head || !*head)
		return;
	cur = *head;
	while (cur)
	{
		next = cur->next;
		free(cur->arg);
		free(cur);
		cur = next;
	}
	*head = NULL;
}

static void cmd_destroy(t_cmd **pcmd)
{
	t_cmd *cmd;

	if (!pcmd || !*pcmd)
		return;
	cmd = *pcmd;
	free_argv(cmd->argv);
	redir_clear(&cmd->redirs);
	free(cmd);
	*pcmd = NULL;
}

void pipeline_destroy(t_pipeline **pp)
{
	t_pipeline *pl;
	size_t i;

	if (!pp || !*pp)
		return;
	pl = *pp;
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
	*pp = NULL;
}

#include <stdio.h>

int main(int argc, char **argv, char **envp)
{
	t_shell sh;
	t_pipeline *pl = NULL;
	shell_init(&sh, envp);
	// debug_minishell_readline(&sh);
	// (void)argv;
	(void)argc;
	(void)pl;
	// (void)sh;
	(void)envp;
	// char *s = argv[1];
	// scanf("%hhd", s);

	t_lexout *tokens;
	tokens = tokenize(argv[1]);

	lexer_debug_print(tokens);
	parse_tokens(&sh, tokens, &pl);
	build_pipeline_from_tokens(tokens, &pl);
	debug_print_pipeline(pl, tokens);
	free_lexout(tokens);

	pipeline_destroy(&pl);
	shell_destroy(&sh);
	return (0);
}
