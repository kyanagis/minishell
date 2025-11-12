#include "lexer.h"
#include "parser.h"

t_pipeline	*pl_b_finalize(t_pl_b *pb)
{
	t_pipeline	*pl;
	t_cmd		**cmds;

	if (pb->len == 0)
		return (NULL);
	pl = (t_pipeline *)ft_xcalloc(1, sizeof(t_pipeline));
	cmds = (t_cmd **)ft_xcalloc(pb->len, sizeof(t_cmd *));
	ft_memcpy(cmds, pb->arr, sizeof(t_cmd *) * pb->len);
	pl->ncmds = pb->len;
	pl->cmds = cmds;
	free(pb->arr);
	pb->arr = NULL;
	pb->len = 0;
	pb->cap = 0;
	return (pl);
}
