/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:11:34 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/25 23:21:27 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	give_parent(t_big_token **b_child, t_big_token **parent)
{
	(*b_child)->parent = *parent;
}

void	sub_parse_1(t_big_token **tmp_b, t_info *info, int b_start, int b_len)
{
	int	btok_info[2];

	btok_info[0] = b_start;
	btok_info[1] = b_len;
	parse(&((*tmp_b)->child), info, btok_info);
	if ((*tmp_b)->child)
		give_parent(&((*tmp_b)->child), tmp_b);
}

void	sub_parse_2(t_big_token **tmp, t_big_token **tmp_b, t_info *info)
{
	t_big_token	*b_child;

	b_child = *tmp;
	give_parent(&b_child, tmp_b);
	while (b_child)
	{
		if ((b_child)->par)
			sub_parse_1(&b_child, info,
				(b_child)->ind_tok_start, (b_child)->length);
		(b_child) = (b_child)->sibling;
	}
}

int	access_file(char *file)
{
	if (access(file, F_OK) == 0)
		return (1);
	else
		return (0);
}

int	parse(t_big_token **b_tokens, t_info *info, int btok_info[2])
{
	t_big_token	*tmp_b;
	int			err;

	err = 0;
	err = divide_by_or_and(b_tokens, info, btok_info);
	if (err)
		return (err);
	tmp_b = *b_tokens;
	if (!tmp_b || (!tmp_b->par && tmp_b->type == TOK_CLEAN))
		return (0);
	while (tmp_b)
	{
		if (tmp_b->par)
			sub_parse_1(&tmp_b, info, tmp_b->ind_tok_start, tmp_b->length);
		else if (piped(info, tmp_b->ind_tok_start, tmp_b->length))
		{
			if (divide_by_pipe(&tmp_b, info))
				return (1);
			if (tmp_b->child)
				sub_parse_2(&tmp_b->child, &tmp_b, info);
		}
		tmp_b = tmp_b->sibling;
	}
	return (0);
}
