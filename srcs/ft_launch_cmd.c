/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_launch_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 10:29:00 by nflan             #+#    #+#             */
/*   Updated: 2022/07/26 16:41:51 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_exit_cmd(t_info *info, char *str, int err)
{
	if (err == -4 && str)
	{
		ft_putstr_fd_3("minishell: ", str, ": Is a directory\n", 2);
		err = 126;
	}
	else if (err == 126 && str)
		ft_putstr_fd_3("minishell: ", str, ": Permission denied\n", 2);
	else if (err == 127 && str)
		ft_putstr_fd_3(NULL, str, ": command not found\n", 2);
	else if (err == -1 && str)
		ft_putstr_fd_3(NULL, str, ": execve failure\n", 2);
	if (info)
		ft_free_all(info, info->env);
	info->status = err;
	rl_clear_history();
	exit(info->status);
}

int	ft_fork_par(t_info *info, t_big_token *b_tokens)
{
	pid_t	pid;

	pid = -1;
	pid = fork();
	ft_manage_sig(info, 0, 0);
	if ((int) pid == -1)
		return (ft_error(2, info, NULL));
	else if ((int) pid == 0)
	{
		ft_manage_sig(info, 1, 0);
		rl_clear_history();
		dup2(b_tokens->fdin, STDIN_FILENO);
		dup2(b_tokens->fdout, STDOUT_FILENO);
		rec_exec(info, &(b_tokens)->child, 0);
		ft_exit_cmd(info, NULL, info->status);
	}
	waitpid(pid, &pid, 0);
	ft_manage_sig(info, 2, pid);
	return (info->status);
}

int	ft_change__(t_env *env, t_big_token *b_tokens)
{
	t_env	*tmp;
	int		err;

	err = 0;
	tmp = env;
	if (!tmp)
		return (1);
	while (tmp && ft_strncmp(tmp->name, "_", 2))
		tmp = tmp->next;
	if (tmp)
	{
		err = ft_export_replace(tmp, b_tokens->cmd_args[0], -1);
		if (err)
			return (2);
	}
	return (err);
}

int	ft_do_solo(t_info *info, t_big_token *b, int ret)
{
	pid_t	pid;

	pid = -1;
	ret = ft_commanding(info, b);
	ft_manage_sig(info, 0, 0);
	pid = fork();
	if ((int) pid == -1)
		exit (ft_mal_err(info, info->env, "Fork error\n"));
	else if ((int) pid == 0)
	{
		ft_manage_sig(info, 1, 0);
		dup2(b->fdin, STDIN_FILENO);
		dup2(b->fdout, STDOUT_FILENO);
		rl_clear_history();
		if (ret)
			ft_exit_cmd(info, b->cmd_args[0], ret);
		else
			if (execve(b->cmd_args[0], b->cmd_args, b->envp) == -1)
				exit (ft_error(4, info, b));
	}
	waitpid(pid, &pid, 0);
	ft_manage_sig(info, 2, pid);
	return (info->status);
}

int	ft_launch_cmd(t_info *info, t_big_token *b_tokens)
{
	if (!b_tokens->cmd_args[0])
		return (1);
	if (b_tokens->par == 1)
		return (ft_fork_par(info, b_tokens));
	b_tokens->envp = ft_env_to_tab(info->env, NULL);
	if (!b_tokens->envp)
		exit (ft_mal_err(info, info->env, "Malloc error\n"));
	if (ft_change__(info->env, b_tokens))
		exit (ft_mal_err(info, info->env, "Malloc error\n"));
	if (!ft_check_builtins(b_tokens))
		info->status = ft_builtins(info, b_tokens);
	else if (ft_check_builtins(b_tokens))
		ft_do_solo(info, b_tokens, 0);
	return (info->status);
}
