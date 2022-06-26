/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 17:15:09 by nflan             #+#    #+#             */
/*   Updated: 2022/06/24 17:32:19 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_option_echo(t_big_token *b_tokens, int i)
{
	int	option;
	int	y;

	option = 1;
	if (!b_tokens->cmd_args[i])
		return (1);
	while (b_tokens->cmd_args[i])
	{
		y = -1;
		if ((b_tokens->cmd_args[i][++y] == '-'
			&& b_tokens->cmd_args[i][++y] == 'n'))
		{
			while (b_tokens->cmd_args[i][y] && b_tokens->cmd_args[i][y] == 'n')
				y++;
			if (!b_tokens->cmd_args[i][y])
				option++;
		}
		i++;
	}
	return (option);
}

int	ft_handle_ret(t_big_token *b_tokens, char *ret, int i)
{
	char	*tofree;
	int		y;

	tofree = b_tokens->cmd_args[i];
	y = 0;
	if (!tofree)
		return (free(ret), 1);
	while (tofree[y])
	{
		if (tofree[y + 1] && tofree[y] == '$' && tofree[y + 1] == '?')
		{
			b_tokens->cmd_args[i] = ft_substr(tofree, 0, y);
			b_tokens->cmd_args[i] = ft_strjoiiin_free(b_tokens->cmd_args[i],
					ret, tofree + y + 2, 1);
		}
		y++;
	}
	free(ret);
	free(tofree);
	return (0);
}

int	ft_echo_none(t_big_token *b_tokens, int i)
{
	if (i == 1)
		ft_putstr_fd("\n", b_tokens->fdout);
	return (0);
}

char	*ft_create_echo(t_info *info, t_big_token *b_tokens, char *tmp, int i)
{
	while (b_tokens->cmd_args[i])
	{
		if (ft_strnstr(b_tokens->cmd_args[i], "$?",
				ft_strlen(b_tokens->cmd_args[i])))
			if (ft_handle_ret(b_tokens, ft_itoa(info->status), i))
				return (NULL);
		if (!tmp)
			tmp = ft_strdup(b_tokens->cmd_args[i]);
		else
			tmp = ft_strjoiiin_free(tmp, " ", b_tokens->cmd_args[i], 1);
		if (!tmp)
			return (NULL);
		i++;
	}
	return (tmp);
}

int	ft_echo(t_info *info, t_big_token *b_tokens)
{
	char	*tmp;
	int		i;
	int		option;

	tmp = NULL;
	option = 0;
	i = ft_option_echo(b_tokens, 1);
	if (i > 1)
		option = 1;
	if (!b_tokens->cmd_args[i])
		return (ft_echo_none(b_tokens, i));
	if (b_tokens->cmd_args[i])
		tmp = ft_create_echo(info, b_tokens, tmp, i);
	if (!tmp)
		return (1);
	if (!option)
		tmp = ft_strjoin_free(tmp, "\n", 1);
	ft_putstr_fd(tmp, b_tokens->fdout);
	free(tmp);
	return (0);
}
