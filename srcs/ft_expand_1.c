#include "../include/minishell.h"

/*int	expand(char **str, t_info *info)
{
	int	i;

	i = 0;
	if (str)
	{
		while (*str && (*str)[i])
		{
			if ((*str)[i] == '$')
			{
				if (expand_1(str, &i, info))
					return (1);
			}
			else
				i++;
		}
	}
	return (0);
}

int	dol_expand(t_token **old_tokens, t_info *info, t_big_token *b_tokens)
{
	t_token	*tmp_o;
	int		start;
	int		length;

	start = b_tokens->ind_tok_start;
	length = b_tokens->length;
	tmp_o = *old_tokens;
	move_tok_2_ind(&tmp_o, start);
	while (tmp_o && length--)
	{
		if (tmp_o->token == TOK_WORD || tmp_o->token == TOK_WORD_D_QUOTED)
			if (expand(&tmp_o->value, info))
				return (1);
		tmp_o = tmp_o->next;
	}
	if (ft_expand_args(b_tokens, info))
		return (1);
	return (0);
}

char	*str_join_exp_8(t_token *tmp)
{
	return (ft_strjoiiin(tmp->prev->value, tmp->next->value,
			tmp->next->next->next->value));
}

char	*str_join_exp(t_token **tokens, int ind, int type)
{
	t_token	*tmp;

	tmp = *tokens;
	if (type == 1 || type == 2 || type == 6 || type == 7)
	{
		move_tok_2_ind(&tmp, ind + 2);
		if (type == 1 || type == 2)
			return (ft_strdup(tmp->value));
		return (ft_strjoin(tmp->value, tmp->next->next->value));
	}
	if (type == 3 || type == 8)
	{
		move_tok_2_ind(&tmp, ind + 1);
		if (type == 3)
			return (ft_strjoin(tmp->prev->value, tmp->next->value));
		return (str_join_exp_8(tmp));
	}
	if (type == 4 || type == 5 || type == 9 || type == 10)
	{
		move_tok_2_ind(&tmp, ind + 1);
		if (type == 4 || type == 5)
			return (ft_strdup(tmp->value));
		return (ft_strjoin(tmp->value, tmp->next->next->value));
	}
	return (NULL);
}*/
