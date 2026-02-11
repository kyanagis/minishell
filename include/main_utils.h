/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/30 00:00:00 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_UTILS_H
# define MAIN_UTILS_H

# include "free_table.h"
# include "minishell.h"

void	parse_and_execute(t_shell *sh, t_free_table *table, t_lexout *tokens);
bool	is_eof(char *line);
bool	should_skip_line(t_shell *sh, char *line);

#endif
