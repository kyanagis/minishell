/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/16 23:13:23 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
#define EXPANDER_H

#include "minishell.h"

typedef struct s_expand_work_buf
{
	char *data;
	size_t len;
	size_t cap;
} t_expand_work_buf;

typedef struct s_expand_input
{
	const char *src;
	const unsigned char *mask;
	size_t len;
} t_expand_input;

void init_work_buf(t_expand_work_buf *buf);
void destroy_work_buf(t_expand_work_buf *buf);
bool buf_append_char(t_expand_work_buf *buf, char c);
bool buf_append_str(t_expand_work_buf *buf, const char *s);
bool handle_dollar(t_expand_work_buf *buf, t_shell *sh,
				   const t_expand_input *input, size_t *idx);

bool expand_pipeline(t_shell *sh, const t_lexout *lexer,
					 t_pipeline *pipeline, t_free_table *table);
bool expand_word(t_shell *sh, const t_expand_input *input, char **out);

#endif
