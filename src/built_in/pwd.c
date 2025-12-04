/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakurako <sakurako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 04:34:05 by sakurako          #+#    #+#             */
/*   Updated: 2025/12/05 05:20:16 by sakurako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int ft_pwd(char **args)
{
    char *cwd;

    (void)args;
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