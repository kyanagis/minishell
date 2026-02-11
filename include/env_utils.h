/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/11 00:00:00 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_UTILS_H
# define ENV_UTILS_H

# include "minishell.h"

void	ft_envlst_delone(t_env *lst, void (*del)(void *));
void	free_env_list(t_env **lst, void (*del)(void *));
t_env	*env_new_node(char *str);
void	env_add_back(t_env **head, t_env *new_node);
t_env	*init_env_list(char **envp);

#endif
