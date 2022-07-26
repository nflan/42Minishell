/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:22:43 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/25 23:04:09 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	divide_by_or_and_1(t_token **tmp, t_info *info, int *length, int *i)
{
	int	st_par;
	int	end_par;

	st_par = (*tmp)->index;
	end_par = cl_par_ind(&(info->tokens), (*tmp)->index);
	*length += (end_par - st_par);
	*i += (end_par - st_par);
	move_tok_2_ind(tmp, end_par);
}

static int	divide_by_or_and_2(t_token *t, t_big_token **b, int p[2], t_info *i)
{
	p[1] -= 1;
	if (!ft_strncmp(t->value, "&&", 2))
		add_b_tok_sib_last(b, TOK_LEFT_AND, p, i);
	else if (!ft_strncmp(t->value, "||", 2))
		add_b_tok_sib_last(b, TOK_LEFT_OR, p, i);
	if (!*b)
		return (1);
	p[0] = t->index + 1;
	p[1] = 0;
	return (0);
}

static int	divide_by_or_and_3(int i, t_info *info, t_big_token **b_tokens)
{
	if (i)
		return (1);
	return (handle_par(b_tokens, info));
}

static void	init_dvde(int (*b_tokinfo)[2], int b)
{
	(*b_tokinfo)[0] = b;
	(*b_tokinfo)[1] = 0;
}

int	divide_by_or_and(t_big_token **b_tokens, t_info *info, int btok_info[2])
{
	t_token	*tmp;
	int		i;
	int		b_tokinfo[2];

	i = -1;
	tmp = info->tokens;
	init_dvde(&b_tokinfo, btok_info[0]);
	move_tok_2_ind(&tmp, btok_info[0]);
	while (++i < btok_info[1])
	{
		b_tokinfo[1]++;
		if (tmp->token == TOK_EXPANDER_OP)
			divide_by_or_and_1(&tmp, info, &b_tokinfo[1], &i);
		else if (tmp->token == TOK_OPERATOR && check_divider_type(tmp->value))
			if (divide_by_or_and_2(tmp, b_tokens, b_tokinfo, info))
				return (1);
		tmp = tmp->next;
	}
	if (!*b_tokens && !piped(info, btok_info[0], btok_info[1]))
		i = add_b_tok_sib_last(b_tokens, TOK_CLEAN, btok_info, info);
	else if (!*b_tokens && piped(info, btok_info[0], btok_info[1]))
		i = add_b_tok_sib_last(b_tokens, TOK_CLEAN_PIPED, btok_info, info);
	else
		i = add_b_tok_sib_last(b_tokens, TOK_LAST, b_tokinfo, info);
	return (divide_by_or_and_3(i, info, b_tokens));
}
