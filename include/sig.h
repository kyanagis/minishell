/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 03:13:27 by sakurako          #+#    #+#             */
/*   Updated: 2026/02/16 23:25:27 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIG_H
# define SIG_H

# include <signal.h>
# include <sys/types.h>
# include <stdio.h>
# include <sys/wait.h>
# include "libft.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>

extern volatile sig_atomic_t	g_sig;

void	handle_sigint(int sig);
void	init_signals(void);
void	set_default_signals(void);
void	set_ignore_signals(void);
int		wait_child_process(pid_t pid);

#endif
