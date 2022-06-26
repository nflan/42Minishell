/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 17:04:35 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/24 17:45:25 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

char *expand_join(char *s1, char *s2, char *s3)
{
	int len;
	int i;
	int j;
	int k;
	char *ret;

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	ret = malloc((len + 1) * sizeof(char));
	if (!ret)
		return (NULL);
	i = 0;
	while (s1 && s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j])
	{
		ret[i + j] = s2[j];
		j++;
	}
	k = 0;
	while (s3 && s3[k])
	{
		ret[i + j + k] = s3[k];
		k++;
	}
	ret[i + j + k] = '\0';
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	if (s3)
		free(s3);
	return (ret);
}

char *expand_join_nf(char *s1, char *s2, char *s3)
{
	int len;
	int i;
	int j;
	int k;
	char *ret;

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	ret = malloc((len + 1) * sizeof(char));
	if (!ret)
		return (NULL);
	i = 0;
	while (s1 && s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j])
	{
		ret[i + j] = s2[j];
		j++;
	}
	k = 0;
	while (s3 && s3[k])
	{
		ret[i + j + k] = s3[k];
		k++;
	}
	ret[i + j + k] = '\0';
	return (ret);
}

char *strjoin_4(char *str1, char *str2)
{
	char *ret;

	ret = ft_strjoin(str1, str2);
	free(str1);
	return (ret);
}

char *str_join_exp(t_token **tokens, int ind, int type)
{
	t_token *tmp;

	tmp = *tokens;
	if (type == 1 || type == 2 || type == 6 || type == 7)
	{
		move_tok_2_ind(&tmp, ind + 2);
		if (type == 1 || type == 2)
			return (ft_strdup(tmp->value));
		return (ft_strjoin(tmp->value, tmp->next->next->value));
	}
	if (type == 3 || type == 8)
	{
		move_tok_2_ind(&tmp, ind + 1);
		if (type == 3)
			return (ft_strjoin(tmp->prev->value, tmp->next->value));
		return (expand_join(tmp->prev->value, tmp->next->value, tmp->next->next->next->value));
	}
	if (type == 4 || type == 5 || type == 9 || type == 10)
	{
		move_tok_2_ind(&tmp, ind + 1);
		if (type == 4 || type == 5)
			return (ft_strdup(tmp->value));
		return (ft_strjoin(tmp->value, tmp->next->next->value));
	}
	return (NULL);
}

char *ft_strndup(char *str, int len)
{
	char *ret;
	int i;

	i = 0;
	if (!str)
		return (NULL);
	ret = ft_calloc(sizeof(char), len + 1);
	if (!ret)
		return (NULL);
	while (i < len && str[i])
	{
		ret[i] = str[i];
		i++;
	}
	return (ret);
}

void expand_1(char **str, int *i, t_info *info)
{
	char *s;
	char *tmp[4];
	char *to_look_up;
	int ind_dol;
	int add_shit;

	s = *str;
	ind_dol = *i;
	add_shit = 0;
	to_look_up = NULL;
	tmp[0] = ft_strndup(s, *i);
	(*i)++;
	if (!s[*i] || (!ft_isalpha((int)s[*i]) && !ft_isdigit((int)s[*i])))
		tmp[1] = ft_strdup("$");
	else
	{
		while (s[(*i)] && (ft_isdigit((int)s[*i]) || ft_isalpha((int)s[*i])))
			(*i)++;
		to_look_up = ft_strndup(&(s[ind_dol + 1]), ((*i) - ind_dol - 1));
		tmp[1] = ft_strdup(ft_get_env_value(info, to_look_up));
		if (tmp[1])
			add_shit = ft_strlen(tmp[1]);
	}
	if (!s[*i])
		tmp[2] = NULL;
	else
		tmp[2] = ft_strndup(&(s[ind_dol + ft_strlen(to_look_up) + 1]), (ft_strlen(s) - ind_dol - ft_strlen(to_look_up) - 1));
	free(to_look_up);
	free(*str);
	(*str) = expand_join(tmp[0], tmp[1], tmp[2]);
	if (!(*str))
		return ;
	if (add_shit)
		(*i) = ind_dol + add_shit;
}

void expand(char **str, t_info *info)
{
	int i;

	i = 0;
	while ((*str)[i])
	{
		// if (str[i] == *)
		// ft_wildcard(&(str[i]));
		if ((*str)[i] == '$')
			expand_1(str, &i, info);
		else
			i++;
	}
}

void dol_expand(t_token **old_tokens, t_info *info, int start, int length)
{
	t_token *tmp_o;

	tmp_o = *old_tokens;
	move_tok_2_ind(&tmp_o, start);
	while (tmp_o && length--)
	{
		if (tmp_o->token == TOK_WORD || tmp_o->token == TOK_WORD_D_QUOTED)
			expand(&tmp_o->value, info);
		tmp_o = tmp_o->next;
	}
}

int expanded_toks_check(t_token **tokens)
{
	t_token *tmp;

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

void expanded_toks(t_token **old_tokens, int start, int length)
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
		add_tok_last_bis(&new_tokens, tmp_o->token, tmp_o->quoted, ft_strdup(tmp_o->value));
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
				add_tok_last_bis(&new_tokens, TOK_WORD, 1, new_value);
				move_tok_2_ind(&tmp_o, tmp_o->index + 4);
			}
			else if (exp_check == 6 || exp_check == 7)
			{
				new_value = str_join_exp(&tmp_o, tmp_o->index, exp_check);
				add_tok_last_bis(&new_tokens, TOK_WORD, 1, new_value);
				move_tok_2_ind(&tmp_o, tmp_o->index + 5);
			}
			else if (exp_check == 3 || exp_check == 8)
			{
				printf("oscour\n");
				new_value = str_join_exp(&tmp_o, tmp_o->index, exp_check);
				if (!ft_strncmp(tmp_o->next->value, "\"", 1))
					add_tok_last_bis(&new_tokens, TOK_WORD, 2, new_value);
				else
					add_tok_last_bis(&new_tokens, TOK_WORD, 3, new_value);
				if (exp_check == 3)
					move_tok_2_ind(&tmp_o, tmp_o->index + 4);
				else
					move_tok_2_ind(&tmp_o, tmp_o->index + 5);
			}
			else if (exp_check == 4 || exp_check == 5 || exp_check == 9 || exp_check == 10)
			{
				new_value = str_join_exp(&tmp_o, tmp_o->index, exp_check);
				if (exp_check == 4 || exp_check == 9)
					add_tok_last_bis(&new_tokens, TOK_WORD, 2, new_value);
				else
					add_tok_last_bis(&new_tokens, TOK_WORD, 3, new_value);
				if (exp_check == 4 || exp_check == 5)
					move_tok_2_ind(&tmp_o, tmp_o->index + 3);
				else
					move_tok_2_ind(&tmp_o, tmp_o->index + 4);
			}
			else
			{
				add_tok_last_bis(&new_tokens, tmp_o->token, 0, ft_strdup(tmp_o->value));
				tmp_o = tmp_o->next;
			}
		}
	}
	if (tmp_o)
	{
		while (tmp_o)
		{
			add_tok_last_bis(&new_tokens, tmp_o->token, tmp_o->quoted, ft_strdup(tmp_o->value));
			tmp_o = tmp_o->next;
		}
	}
	if (new_tokens)
	{
		ft_free_tokens(*old_tokens);
		*old_tokens = new_tokens;
	}
}
