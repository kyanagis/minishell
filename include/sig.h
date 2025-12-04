/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakurako <sakurako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 03:13:27 by sakurako          #+#    #+#             */
/*   Updated: 2025/12/05 03:17:20 by sakurako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIG_H
# define SIG_H

# include <signal.h>
# include <sys/wait.h>

extern volatile sig_atomic_t g_sig;

#endif