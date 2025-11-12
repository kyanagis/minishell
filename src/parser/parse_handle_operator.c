#include "lexer.h"
#include "parser.h"

void	handle_operator(t_work *work, t_ctx *ctx, const t_lexout *lx)
{
	work->kind = lx->kind[work->idx];
	if (work->kind == TOK_WORD)
	{
		if (!parse_handle_word(ctx, work, lx))
			return ; // 失敗したら進めない
	}
	else if (work->kind == TOK_LT || work->kind == TOK_GT
		|| work->kind == TOK_DLT || work->kind == TOK_DGT)
	{
		if (!parse_handle_redir_op(ctx, work)) // ここは中で work->ok を触ってる
			return ;
	}
	else if (work->kind == TOK_PIPE)
	{
		if (!parse_handle_pipe(ctx, work))
			return ;
	}
	else
	{
		work->ok = false;
		return ;
	}
	++work->idx; // 成功した時だけ進める
}
