#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

size_t	env_count(char **p)
{
	char	**q;

	if (!p)
		return (0);
	q = p;
	while (*q)
		q++;
	return ((size_t)(q - p));
}

static void	free_env_until(char **head, char **end)
{
	while (head < end)
	{
		free(*head);
		head++;
	}
}

char	**env_dup(char **src)
{
	size_t	n;
	char	**dst;
	char	**d;
	char	**s;

	n = env_count(src);
	dst = (char **)ft_xmalloc(sizeof(char *) * (n + 1));
	d = dst;
	s = src;
	while (s && *s)
	{
		*d = ft_strdup(*s);
		if (!*d)
		{
			free_env_until(dst, d);
			free(dst);
			return (NULL);
		}
		d++;
		s++;
	}
	*d = NULL;
	return (dst);
}

static char	*build_prompt(t_shell *sh)
{
	char	*status;
	char	*tmp;
	char	*res;

	status = ft_itoa(sh->last_status);
	if (!status)
		return (ft_strdup("minishell "));
	tmp = ft_strjoin("minishell:", status); // test 本番では>のみのプロンプトっぽいのでしゅうせいします　.
	free(status);
	if (!tmp)
		return (ft_strdup("minishell> "));
	res = ft_strjoin(tmp, "> ");
	free(tmp);
	if (!res)
		return (ft_strdup("minishell$ "));
	return (res);
}

void	update_prompt(t_shell *sh)
{
	if (sh->prompt)
		free(sh->prompt);
	sh->last_status %= 256;
	sh->prompt = build_prompt(sh);
}

void	shell_init(t_shell *sh, char **envp)
{
	sh->envp = env_dup(envp);
	sh->last_status = 0;
	sh->prompt = NULL;
	update_prompt(sh);
}

void	shell_destroy(t_shell *sh)
{
	size_t	i;

	if (sh->envp)
	{
		i = 0;
		while (sh->envp[i])
		{
			free(sh->envp[i]);
			i++;
		}
		free(sh->envp);
	}
	if (sh->prompt)
		free(sh->prompt);
}
