/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakurako <sakurako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 06:06:13 by sakurako          #+#    #+#             */
/*   Updated: 2025/12/05 07:22:40 by sakurako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "minishell.h"

static int is_numeric(char *str);

int ft_exit(char **argv, t_shell *shell)
{
    int		status;
    
    ft_putendl_fd("exit", STDERR_FILENO);
    if (!argv[1])
        exit(shell->last_status);
    if (!is_numeric(argv[1]))
    {
        ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
        ft_putstr_fd(argv[1], STDERR_FILENO);
        ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
        // メモリ解放忘れそう
        exit(EXIT_NUMERIC_STATUS);
    }
    if (argv[2])
    {
        ft_putstr_fd(MSG_EXIT_TOO_MANY_ARGS, STDERR_FILENO);
        return (ERROR);
    }
    status = ft_atoi(argv[1]);
    exit(status % 256);
    return (NO_ERROR);
}

static int is_numeric(char *str)
{
    if (ft_strlen(str) > 20)
        return (FALSE);
    while (ft_isspace(*str))
        str++;
    if (*str == '+' || *str == '-')
        str++;
    while (*str)
    {
        if (!ft_isdigit(*str))
            return (FALSE);
        str++;
    }
    return (TRUE);
}