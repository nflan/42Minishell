/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:11:13 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/26 16:53:37 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_check_dol(char *str)
{
	int	i;

	i = -1;
	if (str)
		while (str[++i])
			if (str[i] == '$')
				return (1);
	return (0);
}

int	ft_check_expand(t_token *token, int start, int length)
{
	t_token	*tmp;

	tmp = token;
	move_tok_2_ind(&tmp, start);
	if (tmp)
	{
		while (tmp && length--)
		{
			if (tmp->token == TOK_S_QUOTER || tmp->token == TOK_D_QUOTER)
				return (1);
			else if (tmp->token == TOK_WORD)
				if (ft_check_dol(tmp->value))
					return (1);
			tmp = tmp->next;
		}
	}
	return (0);
}

int	ft_check_exp_line(char *str)
{
	int	t;
	int	count;

	t = 0;
	count = 0;
	if (!str)
		return (1);
	while (*str)
	{
		if (*str == '\'' && !t)
			t = 2;
		else if (*str == '\'' && t == 2)
			t = 0;
		else if (*str == '\"' && !t)
			t = 1;
		else if (*str == '\"' && t == 1)
			t = 0;
		else if (*str == '$' && t < 2)
			count++;
		str++;
	}
	return (count);
}
