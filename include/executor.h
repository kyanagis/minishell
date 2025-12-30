/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:55 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 10:35:42 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "minishell.h"
# include "parser.h"
# include "sig.h"
# include "built_in.h"

# define STATUS_CMD_NOT_FOUND 127
# define STATUS_CMD_NOT_EXEC 126
# define STATUS_GENERAL_ERR 1
# define HEREDOC_PROMPT "> "
# define HEREDOC_TMP_THRESHOLD 4096
# define HEREDOC_RAND_SIZE 8

typedef struct s_fd_target
{
	int	in_fd;
	int	out_fd;
}	t_fd_target;

int		execute_commands(t_shell *sh, t_pipeline *pl);

bool	ensure_env_list(t_shell *sh);
bool	sync_envp(t_shell *sh);

int		execute_single(t_shell *sh, t_cmd *cmd);
int		execute_pipeline(t_shell *sh, t_pipeline *pl);
void	execute_child(t_shell *sh, t_cmd *cmd, int prev_read, int pipefd[2]);
ssize_t	run_pipeline(t_shell *sh, t_pipeline *pl, pid_t *pids);

int		execute_external(t_shell *sh, char **argv);
char	*resolve_path(t_shell *sh, const char *cmd, int *err_code);

void	init_fd_target(t_fd_target *tgt, int in_fd, int out_fd);
int		apply_fd_target(t_fd_target *tgt);
void	close_fd_target(t_fd_target *tgt);
int		prepare_redirections(t_shell *sh, t_redir *redir, t_fd_target *tgt);
int		open_heredoc(t_shell *sh, t_redir *redir);

int		wait_children(pid_t *pids, size_t count);
int		status_from_wait(int status);

int		print_error(const char *prefix, const char *subject, int status);
int		simple_error(const char *subject, const char *msg, int status);
void	cmd_not_found(const char *cmd);
bool	is_builtin(const char *name);
void	executor_cleanup_on_error(t_fd_target *tgt, t_free_table *table);

#endif
