/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakurako <sakurako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 03:13:00 by sakurako          #+#    #+#             */
/*   Updated: 2025/12/05 03:13:16 by sakurako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sig.h"
#include "minishell.h"

volatile sig_atomic_t	g_sig = 0;

void handle_sigint(int sig)
{
    g_sig = sig;

    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void init_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    // SIGINT(CTRL-C)
    sa_int.sa_handler = handle_sigint;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);

    //SIGQUIT(CTRL-\)
    sa_quit.sa_handler = SIG_IGN;
    sa_quit.sa_flags = 0;
    sigemptyset(&sa_quit.sa_mask);
    if (sigaction(SIGINT, &sa_int, NULL) == -1
		|| sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		ft_putstr_fd("Error: Failed to set up signal handlers\n", 2);
		exit(1);
	}
}

void set_default_signals(void)
{
    // 子プロセス側のシグナル設定を初期状態に戻す
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

void set_ignore_signals(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

int wait_child_process(pid_t pid)
{
    int status;
    int exit_code;
    int sig;

    // 親プロセスは待機中、シグナルを無視する（表示崩れ防止）
    set_ignore_signals();
    if (waitpid(pid, &status, 0) == -1)
        return (1);
    // 終了ステータスの判定と表示処理
    if (WIFSIGNALED(status)) // シグナルで死んだ場合
    {
        sig = WTERMSIG(status);
        if (sig == SIGINT) // Ctrl-Cなら改行だけ
            write(1, "\n", 1);
        else if (sig == SIGQUIT) // Ctrl-\ならエラーメッセージ
            ft_putstr_fd("Quit (core dumped)\n", 2);
        exit_code = 128 + sig;
    }
    else if (WIFEXITED(status)) // 普通にexitした場合
        exit_code = WEXITSTATUS(status); // exitの引数をそのまま使う
    else
        exit_code = 1;

    return (exit_code);
}