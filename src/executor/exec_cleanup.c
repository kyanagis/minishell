/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 11:00:19 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:17:17 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "free_table.h"

void executor_cleanup_on_error(t_fd_target *tgt, t_free_table *table)
{
	if (tgt)
		close_fd_target(tgt);
	ft_release(table);
}
