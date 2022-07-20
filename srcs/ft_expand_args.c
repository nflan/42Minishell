/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:10:36 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/01 17:57:36 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_noquote_args(t_big_token *b, t_info *info)
{
	int	i;

	i = 0;
	if (b)
	{
		while (b)
		{
			if (b->cmd_args)
			{
				if (ft_check_expand(info->tokens, b->ind_tok_start, b->length))
				{
					while (b->cmd_args[i])
					{
						b->cmd_args[i] = ft_noquote_line(b->cmd_args[i]);
						if (!b->cmd_args[i])
							return (1);
						i++;
					}
				}
			}
			i = 0;
			b = b->sibling;
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

int	ft_expand_args(t_big_token *b, t_info *info)
{
	int	i;

	i = -1;
	if (b)
	{
		while (b)
		{
			if (!ft_check_expand(info->tokens, b->ind_tok_start, b->length))
				return (0);
			while (b->cmd_args[++i])
			{
				if (ft_check_exp_line(b->cmd_args[i]))
				{
					b->cmd_args[i] = ft_expand_l(b->cmd_args[i], info, 0);
					if (!b->cmd_args[i])
						return (1);
				}
			}
			i = -1;
			b = b->sibling;
		}
	}
	return (0);
}
