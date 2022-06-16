/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 11:11:06 by nflan             #+#    #+#             */
/*   Updated: 2022/06/16 16:49:39 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    ft_signal_dfl(int sig)
{
	signal(SIGQUIT, SIG_DFL);
	signal(sig, SIG_DFL);
	signal(SIGINT, &ft_signal_dfl);
}

int	ft_do_pipex(t_info *info, t_big_token *b_tokens)
{
	if (!ft_check_builtins(info, b_tokens))
	{
		info->status = ft_builtins(info, b_tokens);
		if (info)
			ft_free_all(info, info->env);
		exit(info->status);
	}
	else
	{
		if (ft_command(info, b_tokens))
			return (ft_putstr_frror(b_tokens->cmd_args[0], ": command not found\n", 0));
		else
			if (execve(b_tokens->cmd_args[0], b_tokens->cmd_args, b_tokens->envp) == -1)
				return (ft_error(4, info, b_tokens));
	}
	return (info->status);
}

int	ft_pipex(t_info *info, t_big_token *b_tokens)
{
	if (!info->nb_cmd)
	{
//		printf("do pipex\n");
	//	printf("b_tokens->start = %d && length = %d\n", b_tokens->ind_tok_start, b_tokens->length);
//		print_s_tokens(&info->tokens,  b_tokens->ind_tok_start, b_tokens->length);
		close(info->pdes[0]);
		if (b_tokens->rd_inouthd[0])
			dup2(b_tokens->fdin[b_tokens->rd_inouthd[0 - 1]], STDIN_FILENO);
		else
			dup2(b_tokens->fdin[0], STDIN_FILENO);
		dup2(info->pdes[1], STDOUT_FILENO);
	}
	else if (info->nb_cmd && b_tokens->type == TOK_LEFT_PIPE)
	{
	//	printf("pipe to pipe\n");
		dup2(info->pdes[0], STDIN_FILENO);
		dup2(info->pdes[1], STDOUT_FILENO);
		close(info->tmp[0]);
	}
	else
	{
//		printf("pipe end\n");
		dup2(info->pdes[0], STDIN_FILENO);
		if (b_tokens->rd_inouthd[1])
			dup2(b_tokens->fdout[b_tokens->rd_inouthd[0]], STDOUT_FILENO);
		else
			dup2(b_tokens->fdout[b_tokens->rd_inouthd[1]], STDOUT_FILENO);
	}
	if (b_tokens->par == 1)
	{
		rec_exec(info, &(b_tokens)->child, 0);
		ft_exit_cmd(info);
	}
	info->status = ft_do_pipex(info, b_tokens);
	return (0);
}
