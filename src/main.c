#include "minishell.h"
#include "parser.h"
volatile sig_atomic_t g_sig;

void handler(int signum)
{
	g_sig = signum;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_sig = SIGINT;
}

int minishell(t_shell *sh)
{
	char *line;
	t_lexout *tokens;
	ssize_t tmp;

	signal(SIGINT, handler);
	while (1)
	{
		update_prompt(sh); // これ多分いらない
		sh->last_status = 0;
		line = readline(sh->prompt);
		if (!line)
		{
			tmp = write(1, "exit\n", 5);
			(void)tmp;
			break;
		}
		if (!*line) // debug用
		{
			free(line);
			// continue ;
			break;
		}

		if (g_sig == SIGINT)
		{ // Ctrl-C 中断（プロンプト再表示）
			sh->last_status = 130;
			g_sig = 0;
			free(line);
			continue;
		}
		add_history(line);
		tokens = tokenize(line);
		free(line);
		if (!tokens)
		{
			sh->last_status = 1;
			continue;
		}
		if (!parse_tokens(sh, tokens))
		{
			free_lexout(tokens);
			continue;
		}
		// else
		// 	sh->last_status = 0;
		// ここで実行:
		// sh->last_status = execute(sh, tokens);
		free_lexout(tokens);
	}
	shell_destroy(sh);
	return (sh->last_status);
}

int main(int argc, char **argv, char **envp)
{
	t_shell sh;

	(void)argc;
	(void)argv;
	shell_init(&sh, envp);
	return (minishell(&sh));
}
