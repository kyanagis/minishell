/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 22:22:00 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/01 15:18:04 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	perror_exit(const char *msg, int status)
{
	perror(msg);
	exit(status);
}

void	grow_args(char ***pargs, size_t *cap, size_t need)
{
	char	**newv;
	char	**src;
	char	**dst;
	size_t	ncap;

	if (need <= *cap)
		return ;
	ncap = 8;
	if (*cap)
		ncap = *cap * 2;
	while (ncap < need)
		ncap *= 2;
	newv = malloc(sizeof(char *) * (ncap + 1));
	if (!newv)
		perror_exit(ERR_MALLOC, EXIT_FAILURE);
	src = *pargs;
	dst = newv;
	while (src && *src)
		*dst++ = *src++;
	*dst = NULL;
	free(*pargs);
	*pargs = newv;
	*cap = ncap;
}

static void	scan_line(t_lexer *lexer)
{
	char	ch;

	while (lexer->work[lexer->read_pos])
	{
		ch = lexer->work[lexer->read_pos++];
		if (lexer->state == LEX_GEN)
			handle_gen(lexer, ch);
		else if (lexer->state == LEX_SQ)
			handle_squote(lexer, ch);
		else if (lexer->state == LEX_DQ)
			handle_dquote(lexer, ch);
		// else
		// 	handle_dq_esc(lexer, ch);
		// // else if (lexer->state == LEX_ESC)
		// 	handle_esc(lexer, ch);
	}
	if (lexer->token_start != -1)
		push_current_token(lexer);
}

static t_tokens	**dup_args_and_free_work(t_lexer *lexer)
{
	t_tokens	**out;
	size_t		i;

	out = (t_tokens **)malloc(sizeof(t_tokens *) * (lexer->args_len + 1));
	if (!out)
		perror_exit(ERR_MALLOC, EXIT_FAILURE);
	i = 0;
	while (i < lexer->args_len)
	{
		out[i]->value = strdup(lexer->args[i]); // libft
		if (!out[i])
			perror_exit(ERR_MALLOC, EXIT_FAILURE);
		++i;
	}
	out[i]->value = NULL;
	free(lexer->args);
	free(lexer->work);
	return (out);
}

void	ft_putendl_fd(const char *msg, int fd)
{
	write(fd, msg, strlen(msg));
}

static t_tokens	**finalize_lexer(t_lexer *lexer)
{
	if (lexer->state == LEX_SQ || lexer->state == LEX_DQ)
	{
		free(lexer->args);
		free(lexer->work);
		ft_putendl_fd("Error: unclosed quotes", STDERR_FILENO);
		return (NULL);
	}
	if (!lexer->args)
	{
		lexer->args = (char **)malloc(sizeof(t_tokens *));
		if (!lexer->args)
			perror_exit(ERR_MALLOC, EXIT_FAILURE);
		lexer->args[0] = NULL;
		lexer->args_len = 0;
	}
	return (dup_args_and_free_work(lexer));
}

t_tokens	**tokenize_argv(const char *line)
{
	t_lexer	lexer;

	if (!line)
		return (NULL);
	lexer.work = strdup(line); // libft
	if (!lexer.work)
		perraor_exit(ERR_MALLOC, EXIT_FAILURE);
	lexer.args = NULL;
	lexer.read_pos = 0;
	lexer.write_pos = 0;
	lexer.args_len = 0;
	lexer.args_cap = 0;
	lexer.token_start = -1;
	lexer.state = LEX_GEN;
	scan_line(&lexer);
	return (finalize_lexer(&lexer));
}

int	main(void)
{
	char *s = "ss as dawda '    aa aa ' \" awd awd awd w \"";

	char **test = tokenize_argv(s);
	int i = 0;
	while (test[i])
	{
		printf("%s\n", test[i]);
		++i;
	}
}