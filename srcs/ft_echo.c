/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 17:15:09 by nflan             #+#    #+#             */
/*   Updated: 2022/07/24 23:23:47 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_option_echo(t_big_token *b_toks)
{
	int	option;
	int	y;
	int	i;

	i = 0;
	option = 1;
	if (!b_toks->cmd_args[i + 1])
		return (1);
	while (b_toks->cmd_args[++i])
	{
		y = -1;
		if ((b_toks->cmd_args[i][++y] == '-'
			&& b_toks->cmd_args[i][++y] == 'n'))
		{
			while (b_toks->cmd_args[i][y] && b_toks->cmd_args[i][y] == 'n')
				y++;
			if (!b_toks->cmd_args[i][y])
				option++;
			else
				return (option);
		}
		else
			break ;
	}
	return (option);
}

int	ft_echo_none(t_big_token *b_tokens, int i)
{
	if (i == 1)
		ft_putstr_fd("\n", b_tokens->fdout);
	return (0);
}

void	ft_create_echo(t_big_token *b_tokens, int i)
{
	while (b_tokens->cmd_args[i])
	{
		if (b_tokens->cmd_args[i][0])
		{
			ft_putstr_fd(b_tokens->cmd_args[i], b_tokens->fdout);
			if (b_tokens->cmd_args[i + 1])
				ft_putstr_fd(" ", b_tokens->fdout);
		}
		else if (b_tokens->cmd_args[i + 1] && b_tokens->cmd_args[i][0] == '\0')
			ft_putstr_fd(" ", b_tokens->fdout);
		i++;
	}
}

int	ft_echo(t_big_token *b_tokens)
{
	int		i;
	int		option;

	option = 0;
	i = ft_option_echo(b_tokens);
	if (i > 1)
		option = 1;
	if (!b_tokens->cmd_args[i])
		return (ft_echo_none(b_tokens, i));
	if (b_tokens->cmd_args[i])
		ft_create_echo(b_tokens, i);
	if (!option)
		ft_putstr_fd("\n", b_tokens->fdout);
	return (0);
}
