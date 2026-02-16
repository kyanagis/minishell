/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:11 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/16 23:31:21 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "error_messages.h"

int	execute_external(t_shell *sh, char **argv)
{
	char	*path;
	int		err;

	if (!argv || !argv[0])
		return (0);
	err = 0;
	path = resolve_path(sh, argv[0], &err);
	if (!path)
	{
		if (err == STATUS_CMD_NOT_FOUND)
			cmd_not_found(argv[0]);
		else
			print_error(ERR_PREFIX, argv[0], err);
		return (err);
	}
	execve(path, argv, sh->envp);
	err = STATUS_CMD_NOT_EXEC;
	print_error(ERR_PREFIX, path, err);
	free(path);
	return (err);
}
