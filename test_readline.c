#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	char *s;
	while (1)
	{
		s = readline(">");
		printf("%s\n", s);
		add_history(s);
		if (*s == '\\')
			break ;
		free(s);
	}
	return (1);
}