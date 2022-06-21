/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 17:04:35 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/21 17:43:07 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

char	*expand_join(char *s1, char *s2, char *s3)
{
	int		len;
	int		i;
	int		j;
	int		k;
	char	*ret;

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

char	*ft_strndup(char *str, int len)
{
	char	*ret;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	ret = malloc(len +1);
	if (!ret)
		return (NULL);
	while (i < len && str[i])
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

void expand_1(char **str, int *i, t_info *info)
{
	char *s;
	char **tmp;
	char *to_look_up;
	int ind_dol;
	int	add_shit;

	s = *str;
	ind_dol = *i;
	add_shit = 0;
	to_look_up = NULL;
	tmp = ft_calloc(4 , sizeof(char *));
	if (!tmp)
		return;
	tmp[0] = ft_strndup(s, (*i));
	(*i)++;
	printf("%d\n", (*i));
	if (!s[(*i)] || (!ft_isalpha((int)s[(*i)]) && !ft_isdigit((int)s[(*i)])))
		tmp[1] = ft_strdup("$");
	else
	{
		while (s[(*i)] && (ft_isdigit((int)s[(*i)]) || ft_isalpha((int)s[(*i)])))
			(*i)++;
		to_look_up = ft_strndup(&(s[ind_dol + 1]), ((*i) - ind_dol - 1));
		tmp[1] = ft_get_env_value(info, to_look_up);
		if (tmp[1])
			add_shit = ft_strlen(tmp[1]);
	}
	if (!s[((*i))])
		tmp[2] = NULL;
	else
		tmp[2] = ft_strndup(&(s[ind_dol + ft_strlen(to_look_up) + 1]), (ft_strlen(s) - ind_dol - ft_strlen(to_look_up) - 1));
	free(to_look_up);
	free(*str);
	(*str) = expand_join(tmp[0], tmp[1], tmp[2]);
	free(tmp);
	if (add_shit)
		(*i) = ind_dol + add_shit;
}

void expand(char **str, t_info *info)
{
	int i;

	i = 0;
	while (str[i])
	{
		// if (str[i] == *)
		// ft_wildcard(&(str[i]));
		if ((*str)[i] == '$')
		{
			printf("I entered here\n");
			expand_1(str, &i, info);
		}
		else
			i++;
	}
}

void dol_expand(t_token **old_tokenss, t_info *info)
{
	t_token *tmp_o;

	tmp_o = *old_tokens;
	while (tmp_o)
	{
		if (tmp_o->token == TOK_WORD_D_QUOTED)
		{
			printf("I entered here because a d_quoted word: %s\n", tmp_o->value );
			expand(&tmp_o->value, info);
		}
		else if (tmp_o->token == TOK_WORD)
		{
			printf("I entered here because a normal word: %s\n", tmp_o->value );
			expand(&tmp_o->value, info);
		}
		tmp_o = tmp_o->next;
	}
	tmp_o = *old_tokens;
}

int		expanded_toks_check1(tmp **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	if ((tmp->token == TOK_WORD && ft_strlen(tmp->value) ==1 && !ft_strncmp("$", tmp->value, 1))
			&& (tmp->next && ft_strlen(tmp->value) == 1 && tmp->token == TOK_D_QUOTER)
				&& (tmp->next->next && tmp->token == TOK_WORD) )
}

void	expanded_toks(t_token **old_tokens, t_token **new_tokens)
{
	t_token	*tmp_o;
	int		ind_join;
	char	*new_value;

	tmp_o = *old_tokens;
	while (tmp_o)
	{
		if (tmp_o->token == TOK_WORD && ft_strlen(tmp_o->value) == 1 && !ft_strncmp("$", tmp_o, 1))
		{
			ind_join = tmp_o->index;
			if (tmp_o->next && ft_strlen(tmp_o->value) == 1 && tmp_o->token == TOK_D_QUOTER)
				{
					tmp_o = tmp_o->next;
					if (tmp_o->next && tmp_o->token == TOK_WORD)
					{
						new_value = ft_strjoin_d(old_tokens, ind_join);

						move_tok_2_ind(&tmp_o, ind_join + 4);
					}
				}
		}
		else
		{

			tmp_o = tmp_o->next;
		}
	}
}
