/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya <skatsuya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 07:44:48 by skatsuya          #+#    #+#             */
/*   Updated: 2025/12/12 17:46:24 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

void	free_env_list(t_env **lst, void (*del)(void *))
{
	t_env	*temp;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		ft_envlst_delone(*lst, del);
		*lst = temp;
	}
	*lst = NULL;
}

void	ft_envlst_delone(t_env *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->key);
	del(lst->value);
	free(lst);
}
