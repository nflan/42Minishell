/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 11:11:06 by nflan             #+#    #+#             */
/*   Updated: 2022/06/15 11:24:19 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    ft_signal_dfl(int sig)
{
	signal(SIGQUIT, SIG_DFL);
	signal(sig, SIG_DFL);
	signal(SIGINT, &ft_signal_dfl);
}

int	ft_do_pipex(t_info *info, t_cmd *cmd)
{
	if (!ft_check_builtins(info, cmd))
	{
		info->status = ft_builtins(info, cmd);
		if (info)
			ft_free_all(info, info->env);
		if (cmd)
			ft_free_cmd(cmd);
		exit(info->status);
	}
	else
	{
		if (ft_command(info, cmd))
			return (ft_putstr_frror(cmd->cmd_p[0], ": command not found\n", 0));
		else
			if (execve(cmd->cmd_p[0], cmd->cmd_p, cmd->envp) == -1)
				return (ft_error(4, info, cmd));
	}
	return (info->status);
}

int	ft_pipex(t_info *info, t_cmd *cmd, t_big_token *b_tokens, int sib_child)
{
	if (sib_child < 4)
		info->pdes[0] = cmd->fdin;
	if (!info->nb_cmd && (sib_child == 4))
	{
	//	printf("do pipex\n");
	//	printf("b_tokens->start = %d && length = %d\n", b_tokens->ind_tok_start, b_tokens->length);
//		print_s_tokens(&info->tokens,  b_tokens->ind_tok_start, b_tokens->length);
		close(info->pdes[0]);
		dup2(cmd->fdin, STDIN_FILENO);
		dup2(info->pdes[1], STDOUT_FILENO);
	}
	else if (info->nb_cmd && sib_child == 4 && b_tokens->type == TOK_LEFT_PIPE)
	{
	//	printf("pipe to pipe\n");
		dup2(info->pdes[0], STDIN_FILENO);
		dup2(info->pdes[1], STDOUT_FILENO);
		close(info->tmp[0]);
	}
	else
	{
	//	printf("do pipex end\n");
		dup2(info->pdes[0], STDIN_FILENO);
		dup2(cmd->fdout, STDOUT_FILENO);
	}
	if (b_tokens->par == 1)
	{
		rec_exec(info, &(b_tokens)->child, 0);
	//	printf("coucou\n");
		ft_exit_cmd(info, cmd);
	}
	info->status = ft_do_pipex(info, cmd);
	return (0);
}
