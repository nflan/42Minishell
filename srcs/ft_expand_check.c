#include "../include/minishell.h"

int	ft_check_dol(char *str)
{
	int	i;

	i = -1;
	if (str)
		while (str[++i])
			if (str[i] == '$')
				return (1);
	return (0);
}

int	ft_check_expand(t_token *token, int start, int length)
{
	t_token	*tmp;

	tmp = token;
	move_tok_2_ind(&tmp, start);
	if (tmp)
	{
		while (tmp && length--)
		{
			if (tmp->token == TOK_S_QUOTER || tmp->token == TOK_D_QUOTER)
				return (1);
			else if (tmp->token == TOK_WORD)
				if (ft_check_dol(tmp->value))
					return (1);
			tmp = tmp->next;
		}
	}
	return (0);
}
