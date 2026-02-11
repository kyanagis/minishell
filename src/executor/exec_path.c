/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:26 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 10:35:11 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

static int	check_command_path(const char *path)
{
	struct stat	st;

	if (access(path, F_OK) != 0)
		return (STATUS_CMD_NOT_FOUND);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (STATUS_CMD_NOT_EXEC);
	if (access(path, X_OK) != 0)
		return (STATUS_CMD_NOT_EXEC);
	return (0);
}

static char	*join_path(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, cmd);
	free(tmp);
	return (res);
}

static char	*search_in_path(const char *paths, const char *cmd, int *err_code)
{
	char	**dirs;
	char	*full;
	int		check;
	char	**cur;

	dirs = ft_split(paths, ':');
	if (!dirs)
		return (NULL);
	cur = dirs;
	while (*cur)
	{
		full = join_path(*cur, cmd);
		if (!full)
			break ;
		check = check_command_path(full);
		if (check == 0)
			return (free_env(dirs), full);
		if (err_code)
			*err_code = check;
		free(full);
		cur++;
	}
	free_env(dirs);
	return (NULL);
}

char	*resolve_path(t_shell *sh, const char *cmd, int *err_code)
{
	char		*path;
	const char	*paths;
	int			check;

	if (err_code)
		*err_code = STATUS_CMD_NOT_FOUND;
	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		check = check_command_path(cmd);
		if (err_code)
			*err_code = check;
		if (check != 0)
			return (NULL);
		return (ft_strdup(cmd));
	}
	paths = get_envp_value(sh, "PATH");
	if (!paths)
		return (NULL);
	path = search_in_path(paths, cmd, err_code);
	return (path);
}
