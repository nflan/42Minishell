/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 17:04:35 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/28 11:50:05 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

/*int	expand_1(char **str, int *i, t_info *info)
{
	char	*s;
	char	*tmp[4];
	char	*to_look_up;
	int		ind_dol;
	int		add_shit;

	s = *str;
	ind_dol = *i;
	add_shit = 0;
	to_look_up = NULL;
	tmp[0] = ft_strndup(s, *i);
	if (!tmp[0])
		return (1);
	(*i)++;
	if (!s[*i] || (!ft_isalpha((int)s[*i]) && !ft_isdigit((int)s[*i]) && s[*i] != '?'))
		tmp[1] = ft_strdup("$");
	else
	{
		if (s[(*i)] && (ft_isdigit((int)s[*i]) || ft_isalpha((int)s[*i])))
			while (s[(*i)] && (ft_isdigit((int)s[*i]) || ft_isalpha((int)s[*i])))
				(*i)++;
		else if (s[*i] && s[*i] == '?')
		{
			tmp[1] = ft_itoa(info->status);
			(*i)++;
		}
		to_look_up = ft_strndup(&(s[ind_dol + 1]), ((*i) - ind_dol - 1));
		if (!to_look_up)
			return (free(tmp[0]), 1);
		if (s[*i - 1] != '?' && ft_get_env_value(info, to_look_up))
			tmp[1] = ft_strdup(ft_get_env_value(info, to_look_up));
		else
			tmp[1] = ft_strdup(to_look_up);
		if (tmp[1])
			add_shit = ft_strlen(tmp[1]);
	}
	if (!tmp[1])
	{
		if (to_look_up)
			free(to_look_up);
		return (free(tmp[0]), 1);
	}
	if (!s[*i])
		tmp[2] = NULL;
	else
	{
		tmp[2] = ft_strndup(&(s[ind_dol + ft_strlen(to_look_up) + 1]), (ft_strlen(s) - ind_dol - ft_strlen(to_look_up) - 1));
		if (to_look_up)
			free(to_look_up);
		if (!tmp[2])
			return (free(tmp[0]), free(tmp[1]), 1);
	}
	free(*str);
	(*str) = ft_strjoiiin(tmp[0], tmp[1], tmp[2]);
	free(tmp[0]);
	free(tmp[1]);
	free(tmp[2]);
	if (!(*str))
		return (1);
	if (add_shit)
		(*i) = ind_dol + add_shit - 1;
	return (0);
}

int	expanded_toks_check(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	if ((tmp->token == TOK_WORD && ft_strlen(tmp->value) == 1 && !ft_strncmp("$", tmp->value, 1)) && (tmp->next && ft_strlen(tmp->next->value) == 1 && tmp->next->token == TOK_D_QUOTER) && (tmp->next->next && tmp->next->next->token == TOK_WORD_D_QUOTED))
	{
		move_tok_2_ind(&tmp, tmp->index + 4);
		if (tmp && tmp->token == TOK_WORD)
			return (6);
		return (1);
	}
	if ((tmp->token == TOK_WORD && ft_strlen(tmp->value) == 1 && !ft_strncmp("$", tmp->value, 1)) && (tmp->next && ft_strlen(tmp->next->value) == 1 && tmp->next->token == TOK_S_QUOTER) && (tmp->next->next && tmp->next->next->token == TOK_WORD_S_QUOTED))
	{
		move_tok_2_ind(&tmp, tmp->index + 4);
		if (tmp && tmp->token == TOK_WORD)
			return (7);
		return (2);
	}
	if (tmp->token == TOK_WORD && (tmp->next && ft_strlen(tmp->next->value) == 1 && (tmp->next->token == TOK_D_QUOTER || tmp->next->token == TOK_S_QUOTER)) && (tmp->next->next && (tmp->next->next->token == TOK_WORD_S_QUOTED || tmp->next->next->token == TOK_WORD_D_QUOTED)))
	{
		move_tok_2_ind(&tmp, tmp->index + 4);
		if (tmp && tmp->token == TOK_WORD)
			return (8);
		return (3);
	}
	if (((tmp->token == TOK_D_QUOTER || tmp->token == TOK_S_QUOTER) && ft_strlen(tmp->value) == 1) && (tmp->next && (tmp->next->token == TOK_WORD_S_QUOTED || tmp->next->token == TOK_WORD_D_QUOTED)))
	{
		if (tmp->token == TOK_D_QUOTER)
		{
			move_tok_2_ind(&tmp, tmp->index + 3);
			if (tmp && tmp->token == TOK_WORD)
				return (9);
			return (4);
		}
		if (tmp->token == TOK_S_QUOTER)
		{
			move_tok_2_ind(&tmp, tmp->index + 3);
			if (tmp && tmp->token == TOK_WORD)
				return (10);
			return (5);
		}
	}
	return (0);
}

int	expanded_toks(t_token **old_tokens, int start, int length)
{
	t_token	*new_tokens;
	t_token	*tmp_o;
	char	*new_value;
	int		exp_check;
	int		i;

	i = 0;
	new_tokens = NULL;
	tmp_o = *old_tokens;
	while (tmp_o && i < start)
	{
		if (add_tok_last_bis(&new_tokens, tmp_o->token, tmp_o->quoted, ft_strdup(tmp_o->value)))
			return (1);
		tmp_o = tmp_o->next;
	}
	if (tmp_o)
	{
		while (tmp_o && length--)
		{
			exp_check = expanded_toks_check(&tmp_o);
			if (exp_check == 1 || exp_check == 2)
			{
				new_value = str_join_exp(&tmp_o, tmp_o->index, exp_check);
				if (add_tok_last_bis(&new_tokens, TOK_WORD, 1, new_value))
					return (1);
				move_tok_2_ind(&tmp_o, tmp_o->index + 4);
			}
			else if (exp_check == 6 || exp_check == 7)
			{
				new_value = str_join_exp(&tmp_o, tmp_o->index, exp_check);
				if (add_tok_last_bis(&new_tokens, TOK_WORD, 1, new_value))
					return (1);
				move_tok_2_ind(&tmp_o, tmp_o->index + 5);
			}
			else if (exp_check == 3 || exp_check == 8)
			{
				new_value = str_join_exp(&tmp_o, tmp_o->index, exp_check);
				if (!ft_strncmp(tmp_o->next->value, "\"", 1))
				{
					if (add_tok_last_bis(&new_tokens, TOK_WORD, 2, new_value))
						return (1);
				}
				else
					if (add_tok_last_bis(&new_tokens, TOK_WORD, 3, new_value))
						return (1);
				if (exp_check == 3)
					move_tok_2_ind(&tmp_o, tmp_o->index + 4);
				else
					move_tok_2_ind(&tmp_o, tmp_o->index + 5);
			}
			else if (exp_check == 4 || exp_check == 5 || exp_check == 9 || exp_check == 10)
			{
				new_value = str_join_exp(&tmp_o, tmp_o->index, exp_check);
				if (exp_check == 4 || exp_check == 9)
				{
					if (add_tok_last_bis(&new_tokens, TOK_WORD, 2, new_value))
						return (1);
				}
				else
					if (add_tok_last_bis(&new_tokens, TOK_WORD, 3, new_value))
						return (1);
				if (exp_check == 4 || exp_check == 5)
					move_tok_2_ind(&tmp_o, tmp_o->index + 3);
				else
					move_tok_2_ind(&tmp_o, tmp_o->index + 4);
			}
			else
			{
				if (add_tok_last_bis(&new_tokens, tmp_o->token, 0, ft_strdup(tmp_o->value)))
					return (1);
				tmp_o = tmp_o->next;
			}
		}
	}
	if (tmp_o)
	{
		while (tmp_o)
		{
			if (add_tok_last_bis(&new_tokens, tmp_o->token, tmp_o->quoted, ft_strdup(tmp_o->value)))
				return (1);
			tmp_o = tmp_o->next;
		}
	}
	if (new_tokens)
	{
		if (old_tokens)
			ft_free_tokens(*old_tokens);
		*old_tokens = new_tokens;
	}
	return (0);
}*/
