/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 23:40:54 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/05 02:36:31 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	size_t	src_len;
	char	*p;
	char	*head;

	src_len = ft_strlen(s);
	p = ft_xmalloc(n + 1);
	head = p;
	while (src_len--)
		*p++ = *s++;
	return (head);
}
