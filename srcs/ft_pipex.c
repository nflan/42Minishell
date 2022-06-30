/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 11:11:06 by nflan             #+#    #+#             */
/*   Updated: 2022/06/27 23:46:14 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_do_pipex(t_info *info, t_big_token *b_tokens)
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
		if (ft_command(info, b_tokens))
			ft_exit_cmd(info, b_tokens->cmd_args[0], 127);
		else
		{
			if (info->nb_cmd && ft_strnstr(b_tokens->cmd_args[0],
					"cat", ft_strlen(b_tokens->cmd_args[0])))
				wait(NULL);
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
		dup2(b_tokens->fdin, STDIN_FILENO);
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
	{
		dup2(info->pdes[0], STDIN_FILENO);
		dup2(b_tokens->fdout, STDOUT_FILENO);
	}
	if (b_tokens->par == 1)
	{
		rec_exec(info, &(b_tokens)->child, 0);
		ft_exit_cmd(info, NULL, 0);
	}
	info->status = ft_do_pipex(info, b_tokens);
	return (0);
}

int	ft_launch_cmd_pipex(t_info *info, t_big_token *b_tokens, int pid)
{
	pid = -1;
	if (ft_lead_fd(info, b_tokens))
		return (ft_putstr_error("FD problem\n"));
	b_tokens->envp = ft_env_to_tab(info->env);
	pid = fork();
	ft_manage_sig(0);
	if ((int) pid == -1)
		return (ft_error(2, info, NULL));
	else if ((int) pid == 0)
	{
		ft_manage_sig(1);
		if (ft_pipex(info, b_tokens))
			return (ft_free_cmd(b_tokens), 1);
		ft_exit_cmd(info, NULL, 0);
	}
	ft_manage_sig(2);
	ft_close_cmd(info, b_tokens, pid);
	return (info->status);
}

int	ft_exec_pipex(t_info *info, t_big_token *b_tokens, int *pid)
{
	int			i;
	t_big_token	*tmp;

	i = 0;
	tmp = b_tokens;
	while (tmp)
	{
		if (ft_wash_btoken(info, tmp))
			return (2147483647);
		if (tmp->sc == -1)
		{
			if (ft_add_wildcards(info, tmp))
				return (ft_putstr_error("Wildcards error\n"));
			ft_launch_cmd_pipex(info, tmp, pid[i]);
			info->nb_cmd++;
			tmp->sc = info->status;
			i++;
		}
		b_tokens->sc = tmp->sc;
		tmp = tmp->sibling;
	}
	return (0);
}

int	ft_init_pipex(t_info *info, t_big_token *b_tokens)
{
	t_big_token	*tmp_b;
	int			i;

	tmp_b = b_tokens;
	i = 0;
	while (tmp_b && ++i)
		tmp_b = tmp_b->sibling;
	info->pid = ft_calloc(sizeof(int), i);
	if (!info->pid)
		return (1);
	tmp_b = b_tokens;
	if (pipe(info->pdes) == -1)
		return (free(info->pid), ft_error(5, info, NULL));
	if (ft_exec_pipex(info, b_tokens, info->pid) == 2147483647)
		return (free(info->pid), 2147483647);
	i = -1;
	while (++i < info->nb_cmd)
		waitpid(info->pid[i], &info->pid[i], 0);
	if (info->pid)
		free(info->pid);
	info->pid = NULL;
	return (0);
}
