/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_1_tool.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:45:04 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/28 21:07:42 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	len_ll_list(t_token *tok_list)
{
	int	len;

	len = 0;
	if (!tok_list)
		return (len);
	while (tok_list)
	{
		len++;
		tok_list = tok_list->next;
	}
	return (len);
}

t_token	*ft_create_token(t_tok_type tok_type, int length, int i)
{
	t_token	*tok;

	tok = ft_calloc(sizeof(t_token), 1);
	if (!tok)
		return (ft_putstr_error("Error in create_token in "), NULL);
	tok->token = tok_type;
	tok->length = length;
	tok->start = i;
	return (tok);
}

t_token	*create_tok_bis(t_tok_type tok_type, int quoted, char *value)
{
	t_token	*tok;

	tok = ft_calloc(sizeof(t_token), 1);
	if (!tok)
		return (ft_putstr_error("Error in create_token bis in "), NULL);
	tok->token = tok_type;
	tok->quoted = quoted;
	tok->value = value;
	return (tok);
}
