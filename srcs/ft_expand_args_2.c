/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_args_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 18:29:31 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/26 12:06:52 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

char	*ms_tool(int *a, char **str, int b, int type)
{
	if (type == 1)
	{
		*a = -1;
		*str = NULL;
		if (b == 0)
			*a = 0;
	}
	else
	{
		if (*a >= b)
			*a = 0;
	}
	return (*str);
}

char	*ft_minisplit(char *str)
{
	static int	k = 0;
	char		*tmp;
	int			s;

	ms_tool(&s, &tmp, k, 1);
	if (str)
	{
		while (str[k])
		{
			if (str[k] == ' ' && !ft_postype(str, k) && s == -1)
				s = k + 1;
			else if (str[k + 1] == '\0'
				|| (str[k] == ' ' && !ft_postype(str, k) && s != -1))
			{
				if (str[k + 1] == '\0')
					k++;
				tmp = ft_substr(str, s, k - s);
				if (!tmp)
					return (NULL);
				break ;
			}
			k++;
		}
	}
	return (ms_tool(&k, &tmp, (int)ft_strlen(str), 2));
}

int	resplit_tool(int (*jl)[2], char **tmp, char *to_copy, int type)
{
	int	j;

	j = (*jl)[0];
	if (type == 1)
		tmp[j] = ft_strdup(to_copy);
	else if (type == 2)
		tmp[j] = ft_minisplit(to_copy);
	else if (type == 3)
		tmp[j] = ft_strdup(to_copy);
	if (!tmp[j])
		return (1);
	((*jl)[0])++;
	if (type == 3)
		((*jl)[1])++;
	return (0);
}

void	resplit_tool_2(int (*jl)[2], int i, t_big_token *b, int step)
{
	if (step == 1)
	{
		(*jl)[0] = 0;
		(*jl)[1] = i + 1;
	}
	else
		ft_free_split(b->cmd_args);
}
