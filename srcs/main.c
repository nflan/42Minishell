/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 11:39:37 by nflan             #+#    #+#             */
/*   Updated: 2022/06/27 22:08:23 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_sc;

void	ft_end(t_info *info)
{
	ft_free_env(info->env);
	free(info->home);
	ft_exit(info, NULL);
}

int	ft_launch_minishell(t_info info, char *word)
{
	while (1)
	{
		word = ft_rdline_word(&info);
		if (!word)
			return (ft_free_all(&info, info.env),
				ft_putstr_error("Word Error\n"));
		info.rdline = readline(word);
		free(word);
		if (g_sc)
			info.status = g_sc;
		ft_keep_history(info.rdline);
		if (!info.rdline)
			break ;
		if (!ft_init_info(&info))
		{
			if (info.nb_cmd != 10)
				rec_exec(&info, &info.parse, 0);
			ft_free_all(&info, NULL);
		}
		g_sc = 0;
	}
	ft_end(&info);
}

int	main(int ac, char **av, char **envp)
{
	t_info		info;
	char		*word;

	(void) av;
	g_sc = 0;
	if (ft_init_first(&info, envp))
		return (1);
	if (ac > 1)
		info.nb_cmd = 10;
	return (ft_launch_minishell(info, word));
}
