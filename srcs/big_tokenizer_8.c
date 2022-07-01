/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_8.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:45:15 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/28 19:29:39 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_params(int *adv_steps, int *to_reduce)
{
	(*adv_steps) = 0;
	(*to_reduce) = 0;
}

static int	ft_check_arg(t_token *tmp, int check)
{
	if (check == 1)
	{
		if (tmp->token == TOK_REDIRECTOR_LEFT
			|| tmp->token == TOK_REDIRECTOR_RIGHT)
			return (1);
	}
	else if (check == 2)
		if (tmp->token == TOK_WORD || tmp->token == TOK_WORD_D_QUOTED
			|| tmp->token == TOK_WORD_S_QUOTED)
			return (1);
	return (0);
}

static void	ft_is_red(t_token **tmp, int *red, int *len, int check)
{
	if (check == 1)
	{
		*red = 1;
		tmp[0] = tmp[0]->next;
		*len -= 1;
	}
	else if (check == 2)
	{
		tmp[0] = tmp[0]->next;
		*len -= 1;
	}
	else if (check == 3)
	{
		*red = 0;
		tmp[0] = tmp[0]->next;
		*len -= 1;
	}
	else if (check == 4)
	{
		tmp[0] = tmp[0]->next;
		*len -= 1;
	}
}
//rc[0] = red, rc[1] = count

static void	ft_init_cmd_args(t_token **tmp, t_token **toks, int rc[2])
{
	*tmp = *toks;
	rc[0] = 0;
	rc[1] = 0;
}

void	count_cmd_args(t_big_token **tmp_b, int ind, t_token **tokens, int len)
{
	t_token	*tmp;
	int		rc[2];

	ft_init_cmd_args(&tmp, tokens, rc);
	move_tok_2_ind(&tmp, ind);
	while (tmp && len)
	{
		if (ft_check_arg(tmp, 1))
			ft_is_red(&tmp, &rc[0], &len, 1);
		else if (ft_check_arg(tmp, 2))
		{
			if (!rc[0])
			{
				while (tmp && len && tmp->token != TOK_SEP)
					ft_is_red(&tmp, &rc[0], &len, 2);
				rc[1]++;
			}
			else
				ft_is_red(&tmp, &rc[0], &len, 3);
		}
		else
			ft_is_red(&tmp, &rc[0], &len, 4);
	}
	(*tmp_b)->cmd_args = ft_calloc(sizeof(char *), rc[1] + 1);
	(*tmp_b)->cmd_args_num = rc[1];
}
