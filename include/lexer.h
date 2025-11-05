/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 19:59:35 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/05 09:24:56 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "libft.h"
# include "minishell.h"
# include <stdbool.h>
# include <stddef.h>
# include <unistd.h>

# define QM_NONE 0
# define QM_SQ 1
# define QM_DQ 2

typedef enum e_lexer_state
{
	LEX_GEN,
	LEX_SQ,
	LEX_DQ
}					t_lexer_state;

typedef enum e_tok_kind
{
	TOK_WORD = 0,
	TOK_PIPE,
	TOK_LT,
	TOK_GT,
	TOK_DLT,
	TOK_DGT
}					t_tok_kind;

typedef struct s_lexer
{
	char			*work;
	unsigned char	*mask;
	char			**args;
	unsigned char	**masks;
	size_t			*lens;
	t_tok_kind		*kinds;
	size_t			read_pos;
	size_t			write_pos;
	size_t			args_len;
	size_t			args_cap;
	ssize_t			token_start;
	t_lexer_state	state;
}					t_lexer;

typedef struct s_meta
{
	char			**args;
	unsigned char	**masks;
	size_t			*lens;
	t_tok_kind		*kinds;
}					t_meta;

typedef struct s_lexout
{
	char			**argv;
	unsigned char	**qmask;
	size_t			*len;
	t_tok_kind		*kind;
	size_t			count;
}					t_lexout;

typedef void		(*t_state_handler)(t_lexer *lx, const char *input,
			size_t *index, char c);

t_lexout			*tokenize(const char *line);
void				lexout_free(t_lexout *o);
void				lexer_debug_print(const t_lexout *o);

void				scan_line(t_lexer *lx, const char *input);
void				push_current_token(t_lexer *lx);
void				push_operator_token(t_lexer *lx, const char *op, size_t n,
						t_tok_kind kind);
void				grow_meta(t_lexer *lx, size_t need);

void				handle_gen(t_lexer *lx, const char *input, size_t *index,
						char c);
void				handle_squote(t_lexer *lx, const char *input, size_t *index,
						char c);
void				handle_dquote(t_lexer *lx, const char *input, size_t *index,
						char c);
t_lexout			*finalize_lexer(t_lexer *lx);

int					lx_isblank(int c);
int					is_dq_escapable(char c);
void				cleanup_lexer(t_lexer *lx);

void				free_lexout(t_lexout *lx);
;
#endif
