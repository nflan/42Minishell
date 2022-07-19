/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_launch_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 10:29:00 by nflan             #+#    #+#             */
/*   Updated: 2022/06/28 12:03:00 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_exit_cmd(t_info *info, char *str, int err)
{
	info->status = err;
	if (err == -4 && str)
	{
		ft_putstr_fd_3("minishell: ", str, ": No such file or directory\n", 2);
		info->status = 127;
	}
	else if (err == 126 && str)
		ft_putstr_fd_3("minishell: ", str, ": Permission denied\n", 2);
	else if (err == 127 && str)
		ft_putstr_fd_3(NULL, str, ": command not found\n", 2);
	if (info)
		ft_free_all(info, info->env);
	rl_clear_history();
	exit(info->status);
}

int	ft_fork_par(t_info *info, t_big_token *b_tokens)
{
	pid_t	pid;

	pid = -1;
	pid = fork();
	ft_manage_sig(0);
	if ((int) pid == -1)
		return (ft_error(2, info, NULL));
	else if ((int) pid == 0)
	{
		ft_manage_sig(1);
		rl_clear_history();
		dup2(b_tokens->fdin, STDIN_FILENO);
		dup2(b_tokens->fdout, STDOUT_FILENO);
		rec_exec(info, &(b_tokens)->child, 0);
		ft_exit_cmd(info, NULL, info->status);
	}
	waitpid(pid, &pid, 0);
	ft_manage_sig(2);
	if (WIFEXITED(pid))
		info->status = WEXITSTATUS(pid);
	return (info->status);
}

int	ft_do_solo(t_info *info, t_big_token *b, int ret)
{
	pid_t	pid;

	pid = -1;
	pid = fork();
	ft_manage_sig(0);
	if ((int) pid == -1)
		return (ft_error(2, info, NULL));
	else if ((int) pid == 0)
	{
		ft_manage_sig(1);
		dup2(b->fdin, STDIN_FILENO);
		dup2(b->fdout, STDOUT_FILENO);
		rl_clear_history();
		ret = ft_command(info, b);
		if (ret)
			ft_exit_cmd(info, b->cmd_args[0], ret);
		else
			if (execve(b->cmd_args[0], b->cmd_args, b->envp) == -1)
				exit (ft_error(4, info, b));
	}
	waitpid(pid, &pid, 0);
	ft_manage_sig(2);
	if (WIFEXITED(pid))
		info->status = WEXITSTATUS(pid);
	return (info->status);
}

int	ft_launch_cmd(t_info *info, t_big_token *b_tokens)
{
	if (b_tokens->par == 1)
		return (ft_fork_par(info, b_tokens));
	b_tokens->envp = ft_env_to_tab(info->env);
	if (!ft_check_builtins(b_tokens))
		info->status = ft_builtins(info, b_tokens);
	else if (ft_check_builtins(b_tokens) == 1)
		ft_do_solo(info, b_tokens, 0);
	return (info->status);
}
