/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakurako <sakurako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 05:17:19 by sakurako          #+#    #+#             */
/*   Updated: 2025/12/05 07:14:31 by sakurako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

# define TRUE 1
# define FALSE 0
# define ERROR 1
# define NO_ERROR 0
# define EXIT_NUMERIC_STATUS 255

// message
# define MSG_EXIT_TOO_MANY_ARGS "minishell: exit: too many arguments\n"

# include <unistd.h>
# include <stdio.h>
# include "libft.h"

#endif
