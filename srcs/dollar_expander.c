/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 17:04:35 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/20 23:31:18 by omoudni          ###   ########.fr       */
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
		free(to_look_up);
		if (tmp[1])
			add_shit = ft_strlen(tmp[1]);
	}
	if (!s[((*i)) + 1])
		tmp[2] = NULL;
	else
		tmp[2] = ft_strndup(&(s[ind_dol + 1]), (ft_strlen(s) - ind_dol - 1));
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

void dol_expand(t_token **old_tokens, t_info *info)
{
	t_token *tmp_o;

	tmp_o = *old_tokens;
	while (tmp_o)
	{
		if (tmp_o->token == TOK_WORD_D_QUOTED)
			expand(&tmp_o->value, info);
		else if (tmp_o->token == TOK_WORD)
			expand(&tmp_o->value, info);
		tmp_o = tmp_o->next;
	}
}
