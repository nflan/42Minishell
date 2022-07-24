/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 11:11:06 by nflan             #+#    #+#             */
/*   Updated: 2022/07/25 00:36:39 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_do_pipex(t_info *info, t_big_token *b_tokens, int ret)
{
	rl_clear_history();
	if (!ft_check_builtins(b_tokens))
	{
		info->status = ft_builtins(info, b_tokens);
		if (info)
			ft_free_all(info, info->env);
		exit(info->status);
	}
	else if (ft_check_builtins(b_tokens) == 1)
	{
		ret = ft_command(info, b_tokens);
		if (ret)
			ft_exit_cmd(info, b_tokens->cmd_args[0], ret);
		else
		{
			if (execve(b_tokens->cmd_args[0],
					b_tokens->cmd_args, b_tokens->envp) == -1)
				exit (ft_error(4, info, b_tokens));
		}
	}
	return (info->status);
}

int	ft_pipex(t_info *info, t_big_token *b_tokens)
{
	if (!info->nb_cmd)
	{
		dup2(info->pdes[1], STDOUT_FILENO);
		close(info->pdes[0]);
	}
	else if (info->nb_cmd && b_tokens->type == TOK_LEFT_PIPE)
	{
		dup2(info->pdes[0], STDIN_FILENO);
		dup2(info->pdes[1], STDOUT_FILENO);
		close(info->tmp[0]);
	}
	else
		dup2(info->pdes[0], STDIN_FILENO);
	if (b_tokens->fdin)
		dup2(b_tokens->fdin, STDIN_FILENO);
	if (b_tokens->fdout)
		dup2(b_tokens->fdout, STDOUT_FILENO);
	if (b_tokens->par == 1)
	{
		rec_exec(info, &(b_tokens)->child, 0);
		ft_exit_cmd(info, NULL, 0);
	}
	info->status = ft_do_pipex(info, b_tokens, 0);
	return (0);
}

int	ft_launch_cmd_pipex(t_info *info, t_big_token *b_tokens, pid_t pid)
{
	pid = -1;
	if (!ft_lead_fd(info, b_tokens))
	{
		if (ft_change__(info->env, b_tokens))
			return (info->status = 1, 1);
		b_tokens->envp = ft_env_to_tab(info->env);
		ft_manage_sig(info, 0, 0);
		pid = fork();
		if ((int) pid == -1)
			return (ft_error(2, info, NULL));
		else if ((int) pid == 0)
		{
			ft_manage_sig(info, 1, 0);
			if (ft_pipex(info, b_tokens))
				return (ft_free_cmd(b_tokens), 1);
			ft_exit_cmd(info, NULL, 0);
		}
		info->status = 0;
	}
	ft_close_cmd(info, b_tokens, pid);
	return (info->status);
}

int	ft_exec_pipexx(t_info *info, t_big_token *tmp, pid_t *pid, int *i)
{
	int	err;

	err = ft_expanding(info, tmp);
	if (!err)
		ft_launch_cmd_pipex(info, tmp, pid[*i]);
	else if (err == 1)
		return (ft_close_fd(tmp), 1);
	else if (err == 2)
		info->status = 0;
	(*i)++;
	return (0);
}

int	ft_exec_pipex(t_info *info, t_big_token *b_tokens, pid_t *pid)
{
	int			i;
	t_big_token	*tmp;

	i = 0;
	tmp = b_tokens;
	while (tmp)
	{
		info->status = 0;
		if (ft_open_fd(tmp, info))
			info->status = 1;
		info->nb_cmd++;
		if (tmp->sc == -1)
			if (ft_exec_pipexx(info, tmp, pid, &i))
				return (1);
		tmp->sc = info->status;
		b_tokens->sc = tmp->sc;
		ft_close_fd(tmp);
		tmp = tmp->sibling;
	}
	return (info->status);
}
