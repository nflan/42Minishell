/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 17:18:51 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/25 22:52:52 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_big_token	*ft_create_btoken(t_big_tok_type type, int s, int l, t_info *info)
{
	t_big_token	*tok;

	tok = ft_calloc(sizeof(t_big_token), 1);
	if (!tok)
		return (ft_putstr_error("Malloc error in big_tokenizer_1.c\n"), NULL);
	tok->type = type;
	tok->ind_tok_start = s;
	tok->length = l;
	tok->fdout = 1;
	tok->sc = -1;
	tok->info = info;
	return (tok);
}

int	add_b_tok_sib_last(t_big_token **bl, t_big_tok_type t, int b[2], t_info *i)
{
	t_big_token	*tmp;

	if (!*bl)
	{
		*bl = ft_create_btoken(t, b[0], b[1], i);
		if (!*bl)
			return (ft_putstr_error("Malloc error\n"));
	}
	else
	{
		tmp = *bl;
		while (tmp->sibling)
			tmp = tmp->sibling;
		tmp->sibling = ft_create_btoken(t, b[0], b[1], i);
		if (!tmp->sibling)
			return (ft_putstr_error("Malloc error\n"));
	}
	return (0);
}

int	check_divider_type(char *tmp_value)
{
	if (!ft_strncmp(tmp_value, "&&", 2))
		return (1);
	if (!ft_strncmp(tmp_value, "||", 2))
		return (2);
	return (0);
}

void	move_tok_2_ind(t_token **tokens, int ind)
{
	while (*tokens)
	{
		if ((*tokens)->index == ind)
			return ;
		*tokens = (*tokens)->next;
	}
}
