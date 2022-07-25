/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 19:39:38 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/25 21:22:00 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	lead_or(t_info *i, t_big_token **b, t_big_token *tmp, int faoi[3])
{
	if ((faoi[0] == 1 && tmp->sc == 0) || (faoi[0] == 2 && tmp->sc))
	{
		rec_exec(i, b, faoi[1] + 1);
		return ;
	}
	else if (faoi[0] == 2 && !tmp->sc)
	{
		while (tmp && tmp->type != TOK_LEFT_AND)
			tmp = tmp->sibling;
		if (tmp && tmp->type == TOK_LEFT_AND)
			tmp = tmp->sibling;
	}
	else if (faoi[0] == 1 && tmp->sc)
	{
		while (tmp && tmp->type != TOK_LEFT_OR)
			tmp = tmp->sibling;
		if (tmp && tmp->type == TOK_LEFT_OR)
			tmp = tmp->sibling;
	}
	if (tmp)
		rec_exec(i, &tmp, 0);
}

void	ft_init_exec(t_big_token **b, t_big_token **t, int faoi[3], int ao)
{
	t[0] = *b;
	faoi[0] = 0;
	faoi[1] = ao;
	faoi[2] = 0;
}

int	ft_first_part(t_info *info, t_big_token *tmp_b, int faoi[3])
{
	if (tmp_b->child && tmp_b->sc == -1)
	{
		if (!tmp_b->par)
			rec_exec(info, &(tmp_b->child), 0);
	}
	else if ((!faoi[2] && no_sib_has_child(tmp_b) && tmp_b->parent
			&& tmp_b->sc == -1 && tmp_b->type == TOK_LEFT_PIPE))
		return (1);
	return (0);
}

void	ft_second_part(t_big_token *tmp_b, int faoi[3])
{
	if (tmp_b->type == TOK_LEFT_AND && faoi[2] == faoi[1])
		faoi[0] = 1;
	else if (tmp_b->type == TOK_LEFT_OR && faoi[2] == faoi[1])
		faoi[0] = 2;
	else if (tmp_b->type == TOK_LAST || tmp_b->type == TOK_CLEAN
		|| tmp_b->type == TOK_CLEAN_PIPED)
		faoi[0] = 3;
}

// faoi[0] == fc && faoi[1] == and_or && faoi[2] = i
int	rec_exec(t_info *info, t_big_token **b_tokens, int and_or)
{
	t_big_token	*tmp_b;
	int			faoi[3];

	ft_init_exec(b_tokens, &tmp_b, faoi, and_or);
	while (tmp_b)
	{
		if (ft_first_part(info, tmp_b, faoi))
			return (exec_the_bulk(info, no_sib_has_child(tmp_b), tmp_b));
		if (tmp_b->type == TOK_PIPE_LAST)
			return (exec_the_bulk(info, 4, *b_tokens));
		ft_second_part(tmp_b, faoi);
		if (faoi[0])
			break ;
		tmp_b = tmp_b->sibling;
		faoi[2]++;
	}
	if (tmp_b && tmp_b->sc == -1 && (!tmp_b->child || tmp_b->par))
		if (exec_the_bulk(info, 1, tmp_b))
			return (1);
	if (tmp_b && faoi[0] > 0 && faoi[0] < 3)
		lead_or(info, b_tokens, tmp_b, faoi);
	return (0);
}
