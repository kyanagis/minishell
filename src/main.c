/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 08:42:40 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/05 09:28:41 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	minishell(char **envp)
{
	t_shell		sh;
	char		*line;
	t_lexout	*tokens;

	sh = shell_init(envp);
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
		lexer_debug_print(tokens);
		free_lexout(tokens);
	}
	rl_clear_display();
	shell_destroy(&sh);
	return (sh.last_status);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	return (minishell(envp));
}
