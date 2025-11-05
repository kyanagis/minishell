/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 08:42:40 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/05 14:04:38 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	minishell(t_shell *shell)
{
	char		*line;
	t_lexout	*tokens;

	while (1)
	{
		line = readline("minishell >");
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (!*line)
		{
			free(line);
			continue ;
		}
		add_history(line);
		tokens = tokenize(line);
		free(line);
		parse_tokens();
		lexer_debug_print(tokens);
		free_lexout(tokens);
	}
	// rl_clear_display();
	shell_destroy(shell);
	return (shell->last_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argv;
	(void)argc;
	shell_init(&shell, envp);
	return (minishell(&shell));
}
