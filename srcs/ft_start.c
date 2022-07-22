/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_start.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:14:18 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/20 12:20:42 by nflan            ###   ########.fr       */
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
	if (g_sc)
		info->status = g_sc;
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
	if (err)
	{
		info->status = err;
		if (g_sc == 130)
			info->status = g_sc;
		return (ft_free_all(info, NULL), 1);
	}
	return (0);
}

char	*ft_rdline_word(t_info *info)
{
	char		*word;
	char		*tmp;

	word = NULL;
	tmp = ft_get_env_value(info, "HOME");
	word = getcwd(word, 0);
	if (!word)
		return (NULL);
	if (tmp && !strncmp(tmp, word, ft_strlen(tmp)))
	{
		word = ft_substr_free(word, ft_strlen(tmp), ft_strlen(word)
				- ft_strlen(tmp));
		word = ft_strjoin_free("minishell:~", word, 2);
	}
	else
		word = ft_strjoin_free("minishell:", word, 2);
	word = ft_strjoin_free(word, "$ ", 1);
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
	init_tok_type_tab(&(info->tok_type_tab));
	if (ft_init_env(info, envp))
		return (ft_putstr_error("Error while creating env\n"));
	info->home = ft_strdup(ft_get_env_value(info, "HOME"));
	signal(SIGINT, &ft_signal);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
