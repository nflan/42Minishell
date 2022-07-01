/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_4-1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 03:07:20 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/01 03:07:38 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	count_cmd_args(t_big_token **tmp_b, int ind, t_token **tokens, int len)
{
	t_token *tmp;
	int count;
	int red;

	tmp = *tokens;
	count = 0;
	red = 0;
	move_tok_2_ind(&tmp, ind);
	while (tmp && len)
	{
		if (tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT)
		{
			red = 1;
			tmp = tmp->next;
			len--;
		}
		else if (tmp->token == TOK_WORD || tmp->token == TOK_WORD_D_QUOTED || tmp->token == TOK_WORD_S_QUOTED) // ajout des mots quoted comme args
		{
			if (!red)
			{
				while (tmp && len && tmp->token != TOK_SEP)
				{
					tmp = tmp->next;
					len--;
				}
				count++;
			}
			else
			{
				tmp = tmp->next;
				len--;
				red = 0;
			}
		}
		else
		{
			tmp = tmp->next;
			len--;
		}
	}
	(*tmp_b)->cmd_args = ft_calloc(sizeof(char *), count + 1);
	(*tmp_b)->cmd_args_num = count;
}

void rd_inout_type(char *str, int *type_red)
{
	if (ft_strlen(str) == 1 && !ft_strncmp(str, "<", 1))
		(*type_red) = 1;
	if (ft_strlen(str) == 2 && !ft_strncmp(str, "<<", 2))
		(*type_red) = 2;
	if (ft_strlen(str) == 1 && !ft_strncmp(str, ">", 1))
		(*type_red) = 3;
	if (ft_strlen(str) == 2 && !ft_strncmp(str, ">>", 2))
		(*type_red) = 4;
}


int	handle_par(t_big_token **b_tokens, t_info *info)
{
	t_tmp		tmp;
	t_tab		tab;

	tmp.b = *b_tokens;
	if (is_pipe_in_st_end(tmp.b, info->tokens) || is_red_st_par(tmp.b, info->tokens))
		return (ft_putstr_error("Syntax error 2\n"), 2);
	while (tmp.b)
	{
		handle_par_4(&(tab.params), &tmp.s, tmp.b, info);
		if ((tmp.s->token == TOK_EXPANDER_OP && tmp.b->length > 2))
		{
			if (handle_par_5(&(tmp.s), &(tmp.b), &tab, info))
				return (ft_putstr_error("in handle par "));
		}
		else if (!piped(info, tmp.b->ind_tok_start, tmp.b->length))
		{
			if (handle_dir(&tmp.b, info))
				return (ft_putstr_error("in handle par "));
		}
		else if (handle_piped(&tmp.b, info))
			return (ft_putstr_error("in handle par "));
		tmp.b = tmp.b->sibling;
	}
	return (0);
}
