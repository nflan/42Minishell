/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 11:39:37 by nflan             #+#    #+#             */
/*   Updated: 2022/06/22 18:33:18 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int sc;

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

void	ft_signal(int sig)
{
	signal(sig, SIG_IGN);
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	sc = sig;
	signal(SIGINT, &ft_signal);
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
		return (NULL);
	if (!ft_strncmp(new->name, "SHLVL", 6))
	{
		tmp = ft_itoa(ft_atoi(new->value) + 1);
		free(new->value);
		new->value = ft_strdup_free(tmp);
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
				return (1);
			ft_envadd_back(&new, ptr);
		}
	}
	else
	{
		while (++i < 3)
		{
			ptr = ft_without_env(i);
			if (!ptr)
				return (1);
			ft_envadd_back(&new, ptr);
		}
	}
	info->env = new;
	return (0);
}

int	ft_init_info(t_info *info, int ret)
{
	if (main_agent_O(info))
		return (1);
	info->status = ret;
//	info->nb_cmd = 0;
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

int	main(int ac, char **av, char **envp)
{
	t_info		info;
	char		*word;
	static int	ret = 0;

	(void) av;
	info.nb_cmd = 0;
	info.rdline = NULL;
	if (ac > 1)
		info.nb_cmd = 10;
	if (ft_init_env(&info, envp))
		return (ft_putstr_error("Error create env\n"));
	signal(SIGINT, &ft_signal);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		word = ft_rdline_word(&info);
		if (!word)
			return (1);
		info.rdline = readline(word);
		free(word);
		if (ft_keep_history(info.rdline))
			add_history(info.rdline);
		if (ft_strlen(info.rdline) > 1)
		{
			if (!ft_init_info(&info, ret))
			{
				if (info.nb_cmd != 10)
					rec_exec(&info, &info.parse, 0);
				ret = info.status;
				ft_free_all(&info, NULL);
			}
		}
		else
			break ;
	}
	ft_free_env(info.env);
	ft_exit(&info, NULL);
}
