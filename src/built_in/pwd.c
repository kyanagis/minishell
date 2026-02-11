/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya <skatsuya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 04:34:05 by sakurako          #+#    #+#             */
/*   Updated: 2025/12/12 09:36:48 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int ft_pwd(t_shell *shell, char **argv)
{
    char *cwd;

    (void)shell;
    (void)argv;
    cwd = getcwd(NULL, 0); // NULLを渡した場合、sizeが無視されサイズを自動的に確保
    if (!cwd)
    {
        perror("pwd");
        return (ERROR);
    }
    ft_putstr_fd(cwd, STDOUT_FILENO);
    write(STDOUT_FILENO, "\n", 1);
    free(cwd);
    return (NO_ERROR);
}
