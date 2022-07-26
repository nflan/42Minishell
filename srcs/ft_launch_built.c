/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_launch_built.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:13:34 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/26 12:39:25 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_check_builtins(t_big_token *b_tokens)
{
	int	len;

	if (!b_tokens->cmd_args || !b_tokens->cmd_args[0])
		return (2);
	len = ft_strlen(b_tokens->cmd_args[0]) + 1;
	if (!ft_strncmp(b_tokens->cmd_args[0], "unset", len))
		return (0);
	else if (!ft_strncmp(b_tokens->cmd_args[0], "export", len))
		return (0);
	else if (!ft_strncmp(b_tokens->cmd_args[0], "cd", len))
		return (0);
	else if (!ft_strncmp(b_tokens->cmd_args[0], "exit", len)
		|| !ft_strncmp(b_tokens->cmd_args[0], "exit ", 5))
		return (0);
	else if (!ft_strncmp(b_tokens->cmd_args[0], "pwd", len))
		return (0);
	else if (!ft_strncmp(b_tokens->cmd_args[0], "env", len))
		return (0);
	else if (!ft_strncmp(b_tokens->cmd_args[0], "echo", len))
		return (0);
	return (1);
}

int	ft_builtins(t_info *info, t_big_token *b_tokens)
{
	int	len;

	if (!b_tokens->cmd_args || !b_tokens->cmd_args[0])
		return (2);
	len = ft_strlen(b_tokens->cmd_args[0]) + 1;
	if (!ft_strncmp(b_tokens->cmd_args[0], "unset", len))
		return (ft_unset(info, b_tokens, 0));
	else if (!ft_strncmp(b_tokens->cmd_args[0], "export", len))
		return (ft_export(info, b_tokens));
	else if (!ft_strncmp(b_tokens->cmd_args[0], "cd", len))
		return (ft_cd(info, b_tokens, 0));
	else if (!ft_strncmp(b_tokens->cmd_args[0], "exit", len)
		|| !ft_strncmp(b_tokens->cmd_args[0], "exit ", 5))
		return (ft_exit(info, b_tokens));
	else if (!ft_strncmp(b_tokens->cmd_args[0], "pwd", len))
		return (ft_pwd(b_tokens));
	else if (!ft_strncmp(b_tokens->cmd_args[0], "env", len))
		return (ft_env(info, b_tokens, 0));
	else if (!ft_strncmp(b_tokens->cmd_args[0], "echo", len))
		return (ft_echo(b_tokens));
	return (1);
}
