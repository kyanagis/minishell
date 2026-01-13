/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya < skatsuya@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 03:13:27 by sakurako          #+#    #+#             */
/*   Updated: 2026/01/13 20:01:25 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIG_H
# define SIG_H

# include "minishell.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/wait.h>

extern volatile sig_atomic_t	g_sig;

void							handle_sigint(int sig);
void							init_signals(void);
void							set_default_signals(void);
void							set_ignore_signals(void);
int								wait_child_process(pid_t pid);

#endif
