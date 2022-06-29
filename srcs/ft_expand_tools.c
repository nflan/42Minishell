#include "../include/minishell.h"

void	ft_count_q(char *str, char c, size_t *i, size_t *q)
{
	if (!str)
		return ;
	(*i)++;
	(*q)++;
	while (str[*i] && str[*i] != c)
		(*i)++;
}

size_t	ft_strlen_nq(char *str)
{
	size_t	i;
	size_t	q;

	i = 0;
	q = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			ft_count_q(str, str[i], &i, &q);
		else
			i++;
	}
	return (i - q);
}

void	ft_type(char c, int *t)
{
	if (c == '\'' && !*t)
		*t = 2;
	else if (c == '\'' && *t == 2)
		*t = 0;
	else if (c == '\"' && !*t)
		*t = 1;
	else if (c == '\"' && *t == 1)
		*t = 0;
}
