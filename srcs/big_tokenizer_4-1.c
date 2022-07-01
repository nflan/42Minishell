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

int	handle_par(t_big_token **b_tokens, t_info *info)
{
	t_tmp	tmp;
	t_tab	tab;

	tmp.b = *b_tokens;
	if (is_pipe_in_st_end(tmp.b, info->tokens)
		|| is_red_st_par(tmp.b, info->tokens))
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
