/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 17:18:51 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/24 16:33:49 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_big_token *ft_create_btoken(t_big_tok_type type, int ind_tok_start, int length)
{
	t_big_token *tok;

	tok = ft_calloc(sizeof(t_big_token), 1);
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->ind_tok_start = ind_tok_start;
	tok->length = length;
	tok->fdout = 1;
	tok->sc = -1;
	return (tok);
}

int	add_b_tok_sib_last(t_big_token **b_tok_list, t_big_tok_type type, int start, int length)
{
	t_big_token *tmp;

	if (!*b_tok_list)
	{
		*b_tok_list = ft_create_btoken(type, start, length);
		if (!*b_tok_list)
			return (ft_putstr_error("Malloc error in ft_create_btoken in add_b_tok_sib_last "));
	}
	else
	{
		tmp = *b_tok_list;
		while (tmp->sibling)
			tmp = tmp->sibling;
		tmp->sibling = ft_create_btoken(type, start, length);
		if (!tmp->sibling)
			return (ft_putstr_error("Malloc error in ft_create_btoken in add_b_tok_sib_last "));
	}
	return (0);
}

int check_divider_type(char *tmp_value)
{
	if (!ft_strncmp(tmp_value, "&&", 2))
		return (1);
	if (!ft_strncmp(tmp_value, "||", 2))
		return (2);
	return (0);
}

// int is_cl_2_op(char *value_tok_op, char *value_tok_cl)
// {
// 	if (!ft_strncmp(value_tok_op, "(", 1) && !ft_strncmp(value_tok_cl, ")", 1))
// 		return (1);
// 	if (!ft_strncmp(value_tok_op, "{", 1) && !ft_strncmp(value_tok_cl, "}", 1))
// 		return (2);
// 	return (0);
// }

void move_tok_2_ind(t_token **tokens, int ind)
{
	// printf("\nthe length of the list i'm about to move: %d\n", len_ll_list(*tokens));
	while (*tokens)
	{
		// printf("ind: %d\n", (*tokens)->index);
		if ((*tokens)->index == ind)
			return;
		*tokens = (*tokens)->next;
	}
}
