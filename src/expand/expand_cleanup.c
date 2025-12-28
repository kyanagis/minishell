/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 11:00:10 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 13:01:06 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "free_table.h"

void expander_cleanup_on_error(t_free_table *table)
{
	ft_release(table);
}
