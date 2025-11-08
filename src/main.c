#include "minishell.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <unistd.h>

int	minishell(t_shell *sh)
{
	char		*line;
	t_lexout	*tokens;
	ssize_t		tmp;

	while (1)
	{
		update_prompt(sh);
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
