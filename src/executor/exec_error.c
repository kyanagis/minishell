/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:17 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 10:35:01 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "executor.h"
#include "error_messages.h"

static void	safe_putstr(const char *s)
{
	size_t	len;

	if (!s)
		return ;
	len = ft_strlen(s);
	if (len == 0)
		return ;
	write(STDERR_FILENO, s, len);
}

static const char	*status_message(int status)
{
	if (status == STATUS_CMD_NOT_FOUND)
		return ("command not found");
	if (status == STATUS_CMD_NOT_EXEC)
		return ("Permission denied");
	return ("error");
}

int	print_error(const char *prefix, const char *subject, int status)
{
	const char	*msg;

	msg = status_message(status);
	safe_putstr(prefix);
	safe_putstr(subject);
	safe_putstr(": ");
	safe_putstr(msg);
	safe_putstr("\n");
	return (status);
}

int	simple_error(const char *subject, const char *msg, int status)
{
	safe_putstr(ERR_PREFIX);
	if (subject)
	{
		safe_putstr(subject);
		safe_putstr(": ");
	}
	safe_putstr(msg);
	return (status);
}

void	cmd_not_found(const char *cmd)
{
	safe_putstr(ERR_PREFIX);
	safe_putstr(cmd);
	safe_putstr(": command not found\n");
}
