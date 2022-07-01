/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 21:25:12 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/28 21:28:15 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	init_cl_par_ind(int *nb, t_token **tokens, t_token **tmp)
{
	*nb = 0;
	*tmp = *tokens;
}

static int	ft_check_p(t_token *tmp, int check)
{
	if (check == 1)
		if (tmp->token == TOK_OPERATOR && ft_strlen(tmp->value) == 1
			&& !ft_strncmp(tmp->value, "|", 1))
			return (1);
	return (0);
}

int	cl_par_ind(t_token **tmp_s, int ind_tok)
{
	t_token	*tmp;
	int		nb_op_tok;

	tmp = *tmp_s;
	init_cl_par_ind(&nb_op_tok, tmp_s, &tmp);
	move_tok_2_ind(&tmp, ind_tok);
	while (tmp && tmp->token != TOK_EXPANDER_CL)
	{
		if (tmp->token == TOK_EXPANDER_OP && ft_strlen(tmp->value) == 1
			&& !ft_strncmp("(", tmp->value, 1))
			nb_op_tok++;
		tmp = tmp->next;
	}
	while (tmp && nb_op_tok)
	{
		if (tmp->token == TOK_EXPANDER_CL && ft_strlen(tmp->value) == 1
			&& !ft_strncmp(")", tmp->value, 1))
			nb_op_tok--;
		if (!nb_op_tok)
			return (tmp->index);
		tmp = tmp->next;
	}
	return (-1);
}

int	piped(t_info *info, int start, int length)
{
	t_token	*tmp;
	int		i;

	tmp = info->tokens;
	i = 0;
	move_tok_2_ind(&tmp, start);
	while (i < length && tmp)
	{
		if (ft_check_p(tmp, 1))
			return (1);
		tmp = tmp->next;
		i++;
	}
	return (0);
}

int	sophisticated_piped(t_token **tokens, int start, int length)
{
	t_token	*tmp;
	int		i;
	int		st_end_par[2];

	tmp = *tokens;
	i = 0;
	move_tok_2_ind(&tmp, start);
	while (i < length && tmp)
	{
		if (tmp->token == TOK_EXPANDER_OP)
		{
			st_end_par[0] = tmp->index;
			st_end_par[1] = cl_par_ind(&tmp, tmp->index);
			move_tok_2_ind(&tmp, st_end_par[1]);
			i += (st_end_par[1] - st_end_par[0]);
		}
		else if (ft_check_p(tmp, 1))
			return (1);
		else
		{
			tmp = tmp->next;
			i++;
		}
	}
	return (0);
}
