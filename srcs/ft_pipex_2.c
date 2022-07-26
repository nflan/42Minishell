/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 16:45:57 by nflan             #+#    #+#             */
/*   Updated: 2022/07/26 16:46:02 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_do_pipex(t_info *info, t_big_token *b_tokens, int ret)
{
	if (!ft_check_builtins(b_tokens))
	{
		info->status = ft_builtins(info, b_tokens);
		ft_exit_cmd(info, NULL, info->status);
	}
	else if (ft_check_builtins(b_tokens) == 1)
	{
		rl_clear_history();
		ret = ft_command(info, b_tokens);
		if (ret)
			ft_exit_cmd(info, b_tokens->cmd_args[0], ret);
		else
		{
			if (execve(b_tokens->cmd_args[0],
					b_tokens->cmd_args, b_tokens->envp) == -1)
				ft_exit_cmd(info, b_tokens->cmd_args[0], -1);
		}
	}
	return (info->status);
}
