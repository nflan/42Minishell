/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 13:47:48 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/25 22:46:55 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_tok_type(t_tok_type type, char c, t_info *i, int test)
{
	if (test == 1)
	{
		if (type != TOK_EXPANDER_OP && type != TOK_EXPANDER_CL
			&& type != TOK_S_QUOTER && type != TOK_D_QUOTER && type
			!= TOK_WORD_NULL_S && type != TOK_WORD_NULL_D
			&& (get_real_tok_type(c, &(i->tokens), i->tok_type_tab) == type))
			return (1);
	}
	return (0);
}

int	checko(t_tok_type tok_type)
{
	if (tok_type == TOK_OPERATOR)
		return (1);
	return (0);
}

int	detect_tokens(t_info *in)
{
	unsigned int	t;
	int				i;
	int				length;
	int				start;

	i = 0;
	while (in->rdline[i])
	{
		length = 1;
		start = i;
		t = get_real_tok_type(in->rdline[i], &(in->tokens), in->tok_type_tab);
		i++;
		while (in->rdline[i] && check_tok_type(t, in->rdline[i], in, 1))
		{
			if (checko(t) && strncmp(&(in->rdline[i]), &(in->rdline[i - 1]), 1))
				break ;
			length++;
			i++;
		}
		if (add_tok(&(in->tokens), start, length, t))
			return (1);
	}
	return (0);
}

void	index_toks(t_token **tokens)
{
	t_token	*tmp;
	int		i;

	i = 0;
	tmp = *tokens;
	if (!tmp)
		return ;
	while (tmp)
	{
		tmp->index = i;
		tmp = tmp->next;
		i++;
	}
}
