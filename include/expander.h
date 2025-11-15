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

#endif