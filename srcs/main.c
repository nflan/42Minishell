/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 11:39:37 by nflan             #+#    #+#             */
/*   Updated: 2022/06/23 17:53:24 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int g_sc;

int	ft_keep_history(char *str)
{
	int	i;

	i = 0;
	if (str)
		while (*str && *str == '\n')
			str++;
	if (str)
		while (*str++)
			i++;
	return (i);
}

void	ft_envadd_back(t_env **alst, t_env *new)
{
	t_env	*tmp;

	tmp = NULL;
	tmp = *alst;
	if (alst && new)
	{
		if (*alst == NULL)
			*alst = new;
		else
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
	}
}

int	ft_fill_envnew(t_env *env, char *line)
{
	int	i;
	int	j;

	i = 0;
	j = -1;
	if (!line)
		return (1);
	while (line[i] && line[i] != '=')
		i++;
	env->name = ft_calloc(sizeof(char), i + 1);
	if (!env->name)
		return (1);
	while (++j < i)
		env->name[j] = line[j];
	if (line[j] != '=')
		return (0);
	i = 0;
	j++;
	while (line[j + i])
		i++;
	env->value = ft_calloc(sizeof(char), i + 1);
	if (!env->value)
		return (free(env->name), 1);
	i = 0;
	while (line[j])
		env->value[i++] = line[j++];
	return (0);
}

t_env	*ft_envnew(char *line)
{
	t_env	*new;
	char	*tmp;

	tmp = NULL;
	new = ft_calloc(sizeof(t_env), 1);
	if (!new)
		return (NULL);
	new->next = NULL;
	if (ft_fill_envnew(new, line))
		return (free(new), NULL);
	if (!ft_strncmp(new->name, "SHLVL", 6))
	{
		tmp = ft_itoa(ft_atoi(new->value) + 1);
		free(new->value);
		new->value = ft_strdup_free(tmp);
		if (!new->value)
			return (ft_free_env(new), NULL);
	}
	return (new);
}

t_env	*ft_without_env(int i)
{
	t_env	*new;

	new = ft_calloc(sizeof(t_env), 1);
	if (!new)
		return (NULL);
	new->next = NULL;
	if (i == 0)
	{
		new->name = ft_strdup("PWD");
		new->value = getcwd(new->value, 0);
	}
	else if (i == 1)
	{
		new->name = ft_strdup("SHLVL");
		new->value = ft_strdup("1");
	}
	else if (i == 2)
	{
		new->name = ft_strdup("_");
		new->value = ft_strdup("./minishell");
	}
	if (!new->name || !new->value)
		return (NULL);
	return (new);
}

int	ft_init_env(t_info *info, char **envp)
{
	t_env	*ptr;
	t_env	*new;
	int		i;

	ptr = NULL;
	new = NULL;
	i = -1;
	if (envp[0])
	{
		while (envp[++i])
		{
			ptr = ft_envnew(envp[i]);
			if (!ptr)
				return (ft_free_env(ptr), 1);
			ft_envadd_back(&new, ptr);
		}
	}
	else
	{
		while (++i < 3)
		{
			ptr = ft_without_env(i);
			if (!ptr)
				return (ft_free_env(ptr), 1);
			ft_envadd_back(&new, ptr);
		}
	}
	info->env = new;
	return (0);
}

int	ft_init_info(t_info *info)
{
	int	err;
	
	err = main_agent_O(info);
	if (err)
	{
		info->status = err;
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
		word = ft_substr_free(word, ft_strlen(tmp), ft_strlen(word) - ft_strlen(tmp));
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
	info->old_tokens = NULL;
	info->tokens = NULL;
	if (ft_init_env(info, envp))
		return (ft_putstr_error("Error create env\n"));
	signal(SIGINT, &ft_signal);
	signal(SIGQUIT, SIG_IGN);
	return (0);
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
//	rl_outstream = stderr;
	while (1)
	{
		word = ft_rdline_word(&info);
		if (!word)
			return (ft_free_all(&info, info.env), ft_putstr_error("Word Error\n"));
		info.rdline = readline(word);
		free(word);
		if (g_sc)
			info.status = g_sc;
		if (ft_keep_history(info.rdline))
			add_history(info.rdline);
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
	ft_free_env(info.env);
	ft_exit(&info, NULL);
}
