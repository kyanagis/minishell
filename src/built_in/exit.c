/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 06:06:13 by sakurako          #+#    #+#             */
/*   Updated: 2026/02/16 23:17:49 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

static int	exit_error_numaric(char *str);
static bool	read_sign(const char *str, size_t *idx, int *sign);
static bool	read_number(const char *str, size_t *idx,
				unsigned long long limit, unsigned long long *acc);
static bool	parse_exit_status(const char *str, unsigned char *out);

int	ft_exit(t_shell *shell, char **argv)
{
	unsigned char	exit_status;

	if (!shell || !argv)
		return (ERROR);
	ft_putendl_fd("exit", STDERR_FILENO);
	if (!argv[1])
	{
		shell->should_exit = true;
		return (shell->last_status);
	}
	if (!parse_exit_status(argv[1], &exit_status))
	{
		shell->should_exit = true;
		return (exit_error_numaric(argv[1]));
	}
	if (argv[2])
	{
		ft_putstr_fd(MSG_EXIT_TOO_MANY_ARGS, STDERR_FILENO);
		return (ERROR);
	}
	shell->should_exit = true;
	return ((unsigned char)exit_status);
}

static int	exit_error_numaric(char *str)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	return (EXIT_NUMERIC_STATUS);
}

static bool	parse_exit_status(const char *str, unsigned char *out)
{
	unsigned long long	acc;
	unsigned long long	limit;
	size_t				i;
	int					sign;

	if (!str || !out)
		return (false);
	i = 0;
	if (!read_sign(str, &i, &sign))
		return (false);
	limit = (unsigned long long)LLONG_MAX;
	if (sign < 0)
		limit += 1;
	if (!read_number(str, &i, limit, &acc))
		return (false);
	if (str[i] != '\0')
		return (false);
	if (sign < 0)
		acc = 0ULL - acc;
	*out = (unsigned char)acc;
	return (true);
}

static bool	read_sign(const char *str, size_t *idx, int *sign)
{
	*sign = 1;
	while (str[*idx] && ft_isspace(str[*idx]))
		(*idx)++;
	if (str[*idx] == '+' || str[*idx] == '-')
	{
		if (str[*idx] == '-')
			*sign = -1;
		(*idx)++;
	}
	if (!ft_isdigit(str[*idx]))
		return (false);
	return (true);
}

static bool	read_number(const char *str, size_t *idx,
		unsigned long long limit, unsigned long long *acc)
{
	int	digit;

	*acc = 0;
	while (ft_isdigit(str[*idx]))
	{
		digit = str[*idx] - '0';
		if (*acc > (limit - (unsigned long long)digit) / 10)
			return (false);
		*acc = *acc * 10 + (unsigned long long)digit;
		(*idx)++;
	}
	return (true);
}
