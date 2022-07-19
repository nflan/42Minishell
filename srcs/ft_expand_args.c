/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:10:36 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/01 18:00:11 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_noquote_args(t_big_token *b_tokens)
{
	int	i;

	i = 0;
	if (b_tokens->cmd_args)
	{
		while (b_tokens->cmd_args[i])
		{
			b_tokens->cmd_args[i] = ft_noquote_line(b_tokens->cmd_args[i]);
			if (!b_tokens->cmd_args[i])
				return (1);
			i++;
		}
	}
	return (0);
}

char	*ft_expanded_value(t_info *info, char *tmp)
{
	char	*exp;

	exp = NULL;
	if (tmp && !ft_strncmp(tmp, "?", 2))
		exp = ft_itoa(info->status);
	else
		exp = ft_strdup(ft_get_env_value(info, tmp));
	if (!exp)
		return (free(tmp), NULL);
	return (free(tmp), exp);
}

int	ft_splitlen(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	if (str)
	{
		while (str[i])
		{
			if (str[i] == ' ' && !ft_postype(str, i))
				count++;
			i++;
		}
	}
	return (count);
}

char	*ft_minisplit(char *str)
{
	static int	k = 0;
	char		*tmp;
	int			s;

	s = -1;
	if (k == 0)
		s = 0;
	if (str)
	{
		while (str[k])
		{
			if (str[k] == ' ' && !ft_postype(str, k) && s == -1)
				s = k + 1;
			else if (str[k + 1] == '\0' || (str[k] == ' ' && !ft_postype(str, k) && s != -1))
			{
				if (str[k + 1] == '\0')
					k++;
				tmp = ft_substr(str, s, k - s);
				if (!tmp)
					return (NULL);
				break;
			}
			k++;
		}
	}
	if ((size_t)k >= ft_strlen(str))
		k = 0;
	return (tmp);
}

int	ft_resplit(t_big_token *b, int *i, char *str)
{
	char	**tmp;
	int		j;
	int		l;

	tmp = ft_calloc(sizeof(char *), ft_tablen(b->cmd_args) + ft_splitlen(str));
	j = 0;
	l = *i + 1;
	while (j < *i)
	{
		tmp[j] = ft_strdup(b->cmd_args[j]);
		if (!tmp[j])
			return (1);
		j++;
	}
	*i = *i + ft_splitlen(str) - 1;
	while (j <= *i)
	{
		tmp[j] = ft_minisplit(str);
		if (!tmp[j])
			return (1);
		j++;
	}
	while (b->cmd_args[l])
	{
		tmp[j] = ft_strdup(b->cmd_args[l]);
		if (!tmp[j])
			return (1);
		l++;
		j++;
	}
	ft_free_split(b->cmd_args);
	b->cmd_args = tmp;
	return (0);
}

int	ft_expand_args(t_big_token *b, t_info *info)
{
	int	i;

	i = -1;
	if (b->cmd_args)
	{
		while (b->cmd_args[++i])
		{
			if (ft_check_exp_line(b->cmd_args[i]))
			{
				b->cmd_args[i] = ft_expand_l(b->cmd_args[i], info, 0);
				if (!b->cmd_args[i])
					return (1);
				if (b->cmd_args[i][0] != '\0')
					if (ft_resplit(b, &i, b->cmd_args[i]))
						return (1);
			}
		}
	}
	return (0);
}
