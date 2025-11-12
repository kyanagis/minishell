/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_xstrdup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 04:56:29 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/12 13:37:22 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_xstrdup(const char *s)
{
	char *dst;

	dst = ft_strdup(s);
	if (!dst)
	{
		ft_putendl_fd("ft_strdup", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	return (dst);
}