#include "lexer.h"
#include "parser.h"

bool	cmd_b_is_completely_empty(const t_cmd_b *b)
{
	return (!b || (b->argc == 0 && b->r_head == NULL));
}
t_cmd	*cmd_b_finalize(t_cmd_b *b)
{
	t_cmd	*c;
	char	**argv_final;

	if (!b || (b->argc == 0 && b->r_head == NULL))
		return (NULL);
	argv_final = ft_xcalloc(b->argc + 1, sizeof(char *));
	ft_memcpy(argv_final, b->argv, sizeof(char *) * b->argc);
	free(b->argv);
	b->argv = NULL;
	c = ft_xcalloc(1, sizeof(t_cmd));
	c->argv = argv_final;
	c->redirs = b->r_head;
	b->r_head = NULL;
	b->r_tail = NULL;
	return (c);
}
