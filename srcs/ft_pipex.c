/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 11:11:06 by nflan             #+#    #+#             */
/*   Updated: 2022/06/06 17:58:44 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_pipex_end(t_info *info, t_cmd *cmd)
{
	cmd->child = -1;
	if (!cmd->cmd)
		ft_error(3, info, cmd);
	else
	{
		cmd->child = fork();
		if ((int) cmd->child == -1)
			return (ft_error(2, info, cmd));
		else if ((int) cmd->child == 0)
		{
			dup2(info->pdes[0], STDIN_FILENO);
			dup2(cmd->fdout, STDOUT_FILENO);
			execve(cmd->cmd_p[0], cmd->cmd_p, cmd->envp);
			return (ft_error(4, info, cmd));
		}
		waitpid(cmd->child, &cmd->child, 0);
	}
//	if (cmd->fdin != 0 && cmd->fdin != 1 && cmd->fdin != 2)
//		close(cmd->fdin);
	if (info->pdes[0] > 2)
		close(info->pdes[0]);
	if (WIFEXITED(cmd->child))
		return (WEXITSTATUS(cmd->child));
	return (info->status);
}

int	ft_do_pipe(t_info *info, t_cmd *cmd, int tmp)
{
	cmd->child = fork();
	if ((int) cmd->child == -1)
		return (1);
	else if ((int) cmd->child == 0)
	{
		dup2(info->pdes[0], STDIN_FILENO);
		dup2(info->pdes[1], STDOUT_FILENO);
		close(tmp);
		execve(cmd->cmd_p[0], cmd->cmd_p, cmd->envp);
		return (ft_error(4, info, cmd));
	}
	return (0);
}

int	ft_pipe_to_pipe(t_info *info, t_cmd *cmd)
{
	int		tmp[2];

	cmd->child = -1;
	if (pipe(tmp) == -1)
		return (ft_error(5, info, cmd));
	info->pdes[1] = tmp[1];
	cmd->envp = ft_env_to_tab(info->env);
	if (ft_command(info, cmd))
		ft_error(3, info, cmd);
	else
		ft_do_pipe(info, cmd, tmp[0]);
//	if (cmd->fdin != 0 && cmd->fdin != 1 && cmd->fdin != 2)
//		close(cmd->fdin);
	close(info->pdes[0]);
	info->pdes[0] = tmp[0];
//	if (cmd->fdout != 0 && cmd->fdout != 1 && cmd->fdout != 2)
//		close(cmd->fdout);
	close(info->pdes[1]);
	return (0);
}

void    ft_signal_dfl(int sig)
{
	signal(sig, SIG_DFL);
	signal(SIGINT, &ft_signal_dfl);
}

int	ft_do_pipex(t_info *info, t_cmd *cmd)
{
//	printf("allo\n");
	cmd->child = -1;
	if (!cmd->cmd)
		ft_error(3, info, cmd);
	else
	{
		cmd->child = fork();
		if ((int) cmd->child == -1)
			return (ft_error(2, info, cmd));
		else if ((int) cmd->child == 0)
		{
			dup2(cmd->fdin, STDIN_FILENO);
			dup2(info->pdes[1], STDOUT_FILENO);
			if (info->pdes[0] > 2)
				close(info->pdes[0]);
			signal(SIGQUIT, &ft_signal_dfl);
			execve(cmd->cmd_p[0], cmd->cmd_p, cmd->envp);
			return (ft_error(4, info, cmd));
		}
		waitpid(cmd->child, &cmd->child, 0);
	}
	if (WIFEXITED(cmd->child))
		return (WEXITSTATUS(cmd->child));
	return (0);
}

int	ft_pipex(t_info *info, t_cmd *cmd)
{
	if (pipe(info->pdes) == -1)
		return (ft_error(5, info, cmd));
	if (ft_command(info, cmd))
		return (ft_putstr_frror(cmd->cmd_p[0], ": command not found\n", 0));
	if (cmd->fdin < 0)
		ft_error(1, info, cmd);
	else
	{
	//	if (cmd->fdin)
			info->pdes[0] = cmd->fdin;
	//	if (cmd->fdout)
			info->pdes[1] = cmd->fdout;
		info->status = ft_do_pipex(info, cmd);
	}
	if (info->pdes[1] > 2)
		close(info->pdes[1]);
	return (0);
}
