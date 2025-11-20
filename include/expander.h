#ifndef EXPANDER_H
# define EXPANDER_H

# include "lexer.h"
# include "minishell.h"
# include "parser.h"

typedef struct s_expand_work_buf
{
	char	*data;
	size_t	len;
	size_t	cap;

}			t_expand_work_buf;

bool	expand_pipeline(t_shell *sh, const t_lexout *lexer,
			t_pipeline *pipeline);
bool	expand_word(t_shell *sh, const char *src,
			const unsigned char *mask, size_t len, char **out);

#endif
