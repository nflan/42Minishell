/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_start.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:14:18 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/26 15:47:44 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int	g_sc;

void	ft_keep_history(t_info *info, char *str)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = readline(str);
	free(str);
	if (g_sc == 130)
	{
		info->status = g_sc;
		g_sc = 0;
	}
	if (tmp && info->rdline && strncmp(tmp, info->rdline, ft_strlen(tmp) + 1))
	{
		while (tmp[i] && tmp[i] == '\n')
			i++;
		if (tmp[i])
			add_history(tmp);
	}
	else if (tmp && !info->rdline)
		add_history(tmp);
	if (info->rdline)
		free(info->rdline);
	info->rdline = tmp;
}

int	ft_init_info(t_info *info)
{
	int	err;

	err = main_agent_o(info);
	if (err == 2 || g_sc == 130)
	{
		info->status = err;
		if (g_sc == 130)
		{
			info->status = g_sc;
			g_sc = 0;
		}
		return (ft_free_all(info, NULL), 1);
	}
	else if (err == 1 || g_sc == -999)
		exit (ft_mal_err(info, info->env, NULL));
	return (0);
}

int	ft_rdl_w1(t_info *info, char **word)
{
	if (!ft_get_env_value(info, "PWD"))
	{
		*word = getcwd(*word, 0);
		if (!(*word))
			*word = ft_strdup(".");
	}
	else
		*word = ft_strdup(ft_get_env_value(info, "PWD"));
	if (!*word)
		exit (ft_mal_err(info, info->env, "Malloc error\n"));
	return (0);
}

char	*ft_rdline_word(t_info *info)
{
	char		*word;
	char		*tmp;

	word = NULL;
	tmp = ft_get_env_value(info, "HOME");
	ft_rdl_w1(info, &word);
	if (tmp && !strncmp(tmp, word, ft_strlen(tmp)))
	{
		word = ft_substr_free(word, ft_strlen(tmp), ft_strlen(word)
				- ft_strlen(tmp));
		if (!word)
			exit (ft_mal_err(info, info->env, "Malloc error\n"));
		word = ft_strjoin_free("minishell:~", word, 2);
	}
	else
		word = ft_strjoin_free("minishell:", word, 2);
	if (!word)
		exit (ft_mal_err(info, info->env, "Malloc error\n"));
	word = ft_strjoin_free(word, "$ ", 1);
	if (!word)
		exit (ft_mal_err(info, info->env, "Malloc error\n"));
	return (word);
}

int	ft_init_first(t_info *info, char **envp)
{
	info->nb_cmd = 0;
	info->status = 0;
	info->rdline = NULL;
	info->parse = NULL;
	info->tokens = NULL;
	info->pid = NULL;
	info->pdes[0] = 0;
	info->pdes[1] = 1;
	info->tmp[0] = 0;
	info->tmp[1] = 1;
	init_tok_type_tab(&(info->tok_type_tab));
	if (ft_init_env(info, envp))
		exit (ft_putstr_error("Error while creating env\n"));
	info->home = ft_strdup(ft_get_env_value(info, "HOME"));
	if (!info->home)
		exit (ft_mal_err(info, info->env, "Malloc error\n"));
	signal(SIGINT, &ft_signal);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
