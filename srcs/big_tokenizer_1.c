/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 17:18:51 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/17 12:27:47 by nflan            ###   ########.fr       */
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
	tok->par = 0;
	tok->fdin = 0;
	tok->fdout = -1;
//	tok->rd_inouthd[0] = 0;
//	tok->rd_inouthd[1] = 0;
//	tok->rd_inouthd[2] = 0;
	tok->sc = -1;
//	tok->parent = NULL;
//	tok->child = NULL;
//	tok->sibling = NULL;
	tok->cmd_args_num = 0;
	return (tok);
}

void add_b_tok_sib_last(t_big_token **b_tok_list, t_big_tok_type type, int start, int length)
{
	t_big_token *tmp;
	t_big_token *bef_last;
	int rank_in_list;

	if (!*b_tok_list)
	{
		*b_tok_list = ft_create_btoken(type, start, length);
		rank_in_list = 0;
	}
	else
	{
		tmp = *b_tok_list;
		while (tmp->sibling)
			tmp = tmp->sibling;
		bef_last = tmp;
		bef_last->sibling = ft_create_btoken(type, start, length);
		rank_in_list = 1;
	}
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
