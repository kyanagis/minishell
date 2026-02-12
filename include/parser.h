/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/12 18:47:28 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef struct s_free_table	t_free_table;
typedef bool				t_parse_handler(t_lexout *lx, size_t i);

# define PARSE_HANDLER_COUNT 6

typedef struct s_work_command
{
	char					**argv;
	size_t					argc;
	size_t					cap;
	size_t					*tok_idx_argv;
	t_redir					*r_head;
	t_redir					*r_tail;
}							t_work_command;

typedef struct s_work_pipeline
{
	t_cmd					**arr;
	size_t					len;
	size_t					cap;
}							t_work_pipeline;

typedef struct s_work_context
{
	const t_lexout			*tokens;
	t_work_pipeline			pipeline_builder;
	t_work_command			*current_builder;
	unsigned char			*heredoc_quote_cache;
	bool					expecting_redir_arg;
	t_redir_kind			pending_redir_kind;
}							t_work_context;

typedef struct s_work_state
{
	size_t					index;
	t_tok_kind				kind;
	bool					ok;
	bool					heredoc_delim_quoted;
}							t_work_state;

bool						work_command_is_empty(t_work_command *builder);
void						flush_current_command(t_work_context *ctx,
								t_work_state *state);

t_cmd						*work_command_extract(t_work_command *builder);
t_pipeline					*work_pipeline_build(t_work_pipeline *builder);
void						init_work_context(t_work_context *ctx,
								const t_lexout *tokens);
void						init_work_state(t_work_state *state);
void						dispose_command_builder(t_work_command *builder);
void						dispose_pipeline_builder(t_work_pipeline *builder);
bool						finalize_pipeline_success(t_work_context *ctx,
								t_work_state *state, t_pipeline **out);
bool						parse_tokens(t_shell *sh, t_lexout *tokens,
								t_pipeline **pl, t_free_table *table);
bool						parse_syntax_error(const char *unexpected);
const char					*token_str(t_tok_kind kind);

bool						dgt_syntax_scan(t_lexout *tokens, size_t i);
bool						dlt_syntax_scan(t_lexout *tokens, size_t i);
bool						gt_syntax_scan(t_lexout *tokens, size_t i);
bool						lt_syntax_scan(t_lexout *tokens, size_t i);
void						work_pipeline_push(t_work_pipeline *builder,
								t_cmd *cmd);

bool						handle_word_token(t_work_context *ctx,
								t_work_state *state, const t_lexout *tokens);
bool						handle_redirection_operator(t_work_context *ctx,
								t_work_state *state);
bool						handle_pipe_token(t_work_context *ctx,
								t_work_state *state);
bool						build_pipeline_from_tokens(const t_lexout *tokens,
								t_pipeline **out, t_free_table *table);
void						dispatch_parse_token(t_work_state *state,
								t_work_context *ctx, const t_lexout *tokens);

#endif
