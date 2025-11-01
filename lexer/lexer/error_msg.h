/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 23:03:39 by kyanagis          #+#    #+#             */
/*   Updated: 2025/10/30 16:25:28 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_MSG_H
# define ERROR_MSG_H

# define USAGE "Usage: ./pipex file1 cmd1 cmd2 file2"
# define ERR_ENVP "Error: envp is missing.\n"
# define ERR_PATH_NOT_FOUND "Error: PATH environment variable not found.\n"
# define ERR_MALLOC "malloc"
# define ERR_DUP "dup2"
# define ERR_PIPE "pipe"
# define ERR_FORK "fork"
# define ERR_DEVNULL "/dev/null"
# define ERR_CMD_NOT_FOUND "command not found"

#endif