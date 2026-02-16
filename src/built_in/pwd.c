/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 04:34:05 by sakurako          #+#    #+#             */
/*   Updated: 2026/02/16 23:18:17 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	ft_pwd(t_shell *shell, char **argv)
{
	char	*cwd;

	(void)shell;
	(void)argv;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (ERROR);
	}
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(cwd);
	return (NO_ERROR);
}
