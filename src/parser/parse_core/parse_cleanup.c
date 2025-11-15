#include "parser.h"

static void	free_redirection_list(t_redir *head)
{
	t_redir	*next;

	while (head)
	{
		next = head->next;
		free(head->arg);
		free(head);
		head = next;
	}
}

static void	free_command_args(char **argv, size_t argc)
{
	size_t	i;

	if (!argv)
		return ;
	i = 0;
	while (i < argc)
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	dispose_command_builder(t_work_command *builder)
{
	if (!builder)
		return ;
	free_redirection_list(builder->r_head);
	free_command_args(builder->argv, builder->argc);
	free(builder->tok_idx_argv);
	free(builder);
}

static void	free_pipeline_command(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_redirection_list(cmd->redirs);
	free_command_args(cmd->argv, cmd->argc);
	free(cmd->tok_idx_argv);
	free(cmd);
}

void	dispose_pipeline_builder(t_work_pipeline *builder)
{
	size_t	i;

	if (!builder->arr)
		return ;
	i = 0;
	while (i < builder->len)
	{
		free_pipeline_command(builder->arr[i]);
		i++;
	}
	free(builder->arr);
	builder->arr = NULL;
	builder->len = 0;
	builder->cap = 0;
}
