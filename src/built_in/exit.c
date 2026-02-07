/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya < skatsuya@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 06:06:13 by sakurako          #+#    #+#             */
/*   Updated: 2026/01/20 19:53:50 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "minishell.h"

static int	exit_error_numaric(char *str);
static int	is_numeric(char *str);

int	ft_exit(t_shell *shell, char **argv)
{
	int	status;

	if (!shell || !argv)
		return (ERROR);
	ft_putendl_fd("exit", STDERR_FILENO);
	if (!argv[1])
	{
		shell->should_exit = true;
		return (shell->last_status);
	}
	if (!is_numeric(argv[1]))
	{
		shell->should_exit = true;
		return (exit_error_numaric(argv[1]));
	}
	if (argv[2])
	{
		ft_putstr_fd(MSG_EXIT_TOO_MANY_ARGS, STDERR_FILENO);
		return (ERROR);
	}
	status = ft_atoi(argv[1]);
	shell->should_exit = true;
	return ((unsigned char)status);
}

static int	exit_error_numaric(char *str)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	return (EXIT_NUMERIC_STATUS);
}

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (FALSE);
	if (ft_strlen(str) > 20)
		return (FALSE);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}
