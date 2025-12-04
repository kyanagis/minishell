/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakurako <sakurako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 03:33:54 by sakurako          #+#    #+#             */
/*   Updated: 2025/12/05 05:21:01 by sakurako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

static int is_n_str(const char *str);

int ft_echo(char **argv)
{
    int n_flag;

    argv++;
    n_flag = FALSE;
    while (*argv && is_n_str(*argv))
    {
        n_flag = TRUE;
        argv++;
    }
    while (*argv)
    {
        ft_putstr_fd(*argv, STDOUT_FILENO);
        if (argv[1] != NULL)
            write(STDOUT_FILENO, " ", 1);
        argv++;
    }
    if (!n_flag)
        ft_putstr_fd("\n", STDOUT_FILENO);
    return (0);
}

static int is_n_str(const char *str)
{
    if (ft_strncmp(str, "-n", 2) != 0)
        return (FALSE);
    str = str + 2;
    while (*str == 'n')
        str++;
    if (!*str)
        return (TRUE);
    return (FALSE);
}