#include "minishell.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <unistd.h>

volatile sig_atomic_t	sigsig;

void	handler(int signum)
{
	(void)signum;
	printf("a\n");
	ft_putendl_fd("minishell:0>", STDOUT_FILENO);
	// write(1, "\n", 1);
}

int	minishell(t_shell *sh)
{
	char		*line;
	t_lexout	*tokens;
	ssize_t		tmp;

	signal(SIGINT, handler);
	while (1)
	{
		update_prompt(sh); //これ多分いらない
		sh->last_status = 0;
		line = readline(sh->prompt);
		if (!line)
		{
			tmp = write(1, "exit\n", 5);
			(void)tmp;
			break ;
		}
		if (!*line)
		{
			free(line);
			// continue ;
			break ;
		}
		add_history(line);
		tokens = tokenize(line);
		free(line);
		if (!tokens)
		{
			sh->last_status = 1;
			continue ;
		}
		if (!parse_tokens(sh, tokens))
		{
			free_lexout(tokens);
			continue ;
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

int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;

	(void)argc;
	(void)argv;
	shell_init(&sh, envp);
	return (minishell(&sh));
}
