/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executer_tools.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:07:24 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/22 01:46:49 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	no_sib_has_child(t_big_token *b_tokens)
{
	t_big_token	*tmp_b;

	tmp_b = b_tokens;
	while (tmp_b)
	{
		if (tmp_b->child && tmp_b->sc == -1)
			return (0);
		tmp_b = tmp_b->sibling;
	}
	tmp_b = b_tokens;
	if (tmp_b->type == TOK_CLEAN)
		return (1);
	if (tmp_b->type == TOK_LEFT_OR)
		return (2);
	if (tmp_b->type == TOK_LEFT_AND)
		return (3);
	if (tmp_b->type == TOK_LEFT_PIPE)
		return (4);
	return (0);
}

void	give_parent_sc(t_big_token **child, t_big_token **parent)
{
	(*parent)->sc = (*child)->sc;
	if ((*parent)->parent && ((*parent)->par
			|| (*parent)->type == TOK_LEFT_PIPE))
		give_parent_sc(parent, &(*parent)->parent);
}
