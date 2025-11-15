
#include "parser.h"

bool	expand_pipeline(t_shell *sh, const t_lexout *lexer,
		t_pipeline *pipeline)
{
	size_t	idx;

	if (!sh || !lexer || !pipeline)
		return (false);
	idx = 0;
	while (idx < pipeline->ncmds)
	{
		if (!expand_cmd(sh, lexer, pipeline->cmds[idx]))
			return (false);
		++idx;
	}
	return (true);
}
