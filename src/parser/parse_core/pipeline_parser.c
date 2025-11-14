#include "parser.h"

static bool parse_tokens_loop(const t_lexout *tokens,
							  t_work_context *ctx, t_work_state *state)
{
	while (state->ok && state->index < tokens->count)
		dispatch_parse_token(state, ctx, tokens);
	return (state->ok);
}

static void cleanup_parse_context(t_work_context *ctx)
{
	dispose_command_builder(ctx->current_builder);
	ctx->current_builder = NULL;
	dispose_pipeline_builder(&ctx->pipeline_builder);
	free(ctx->heredoc_quote_cache);
	ctx->heredoc_quote_cache = NULL;
}

// : トークン列を検証しながらパイプライン構築処理をまとめて呼び出す。

bool build_pipeline_from_tokens(const t_lexout *tokens, t_pipeline **out)
{
	t_work_context ctx;
	t_work_state state;

	if (!out)
		return (false);
	*out = NULL;
	if (!tokens || tokens->count == 0)
		return (true);
	init_work_context(&ctx, tokens);
	init_work_state(&state);
	if (!parse_tokens_loop(tokens, &ctx, &state) || !finalize_pipeline_success(&ctx, &state, out))
	{
		cleanup_parse_context(&ctx);
		return (false);
	}
	dispose_command_builder(ctx.current_builder);
	ctx.current_builder = NULL;
	free(ctx.heredoc_quote_cache);
	ctx.heredoc_quote_cache = NULL;
	return (true);
}
