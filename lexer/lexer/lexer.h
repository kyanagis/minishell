/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:24:25 by kyanagis          #+#    #+#             */
/*   Updated: 2025/10/31 22:41:58 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "error_msg.h"
# include <ctype.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef enum e_lexer_state
{
	LEX_GEN,
	LEX_SQ,
	LEX_DQ,
	LEX_ESC,
	LEX_DQ_ESC
}					t_lexer_state;

typedef struct s_lexer
{
	char			*work;
	char			**args;
	size_t			read_pos;
	size_t			write_pos;
	size_t			args_len;
	size_t			args_cap;
	ssize_t			token_start;
	t_lexer_state	state;
}					t_lexer;

typedef struct e_tokens
{
	char			**value;
	t_lexer_state	*state;
}					t_tokens;

void				perror_exit(const char *msg, int status);

int					is_dq_escapable(char c);

void				handle_dq_esc(t_lexer *lexer, char c);
void				handle_dquote(t_lexer *lexer, char c);
void				handle_squote(t_lexer *lexer, char c);
void				handle_esc(t_lexer *lexer, char c);
void				handle_gen(t_lexer *lexer, char c);

void				grow_args(char ***pargs, size_t *cap, size_t need);
void				push_current_token(t_lexer *lxer);

#endif