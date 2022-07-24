/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 16:10:34 by nflan             #+#    #+#             */
/*   Updated: 2022/07/24 22:17:49 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int	g_sc;

void	ft_cmd_signal(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		g_sc = 130;
	}
	if (sig == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)\n", 2);
		g_sc = 131;
	}
}

void	ft_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_sc = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ft_manage_sig(t_info *info, int sig, pid_t pid)
{
	if (sig == 0)
	{
		signal(SIGINT, &ft_cmd_signal);
		signal(SIGQUIT, &ft_cmd_signal);
	}
	else if (sig == 1)
	{
		signal(SIGINT, &ft_signal);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (sig == 2)
	{
		signal(SIGINT, &ft_signal);
		signal(SIGQUIT, SIG_IGN);
		if (WIFEXITED(pid))
			info->status = WEXITSTATUS(pid);
	}
	if (g_sc == 130 || g_sc == 131)
		info->status = g_sc;
}
