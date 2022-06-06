/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 12:32:37 by nflan             #+#    #+#             */
/*   Updated: 2022/06/06 19:02:11 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_do_export(char *line)
{
	char	*tmp;
	int		b;

	b = 0;
	tmp = NULL;
	if (!line)
		return (0);
	if (ft_isdigit(*line) || (*line == '\\' && line[1] == '\\') || *line == '\'' || *line == '-')
	{
		tmp = ft_strjoin("minishell: export: `", line);
		ft_putstr_frror(tmp, "': not a valid identifier", 1);
		ft_putstr_fd("\n", 2);
		return (0);
	}
	while (*line++)
		if (*line == '=')
			b = 1;
	return (b);
}

void	ft_export_new(t_env *env, t_env *tmp, char *line)
{
	tmp = ft_envnew(line);
	if (!tmp)
		return ;
	ft_envadd_back(&env, tmp);
}

void	ft_export_replace(t_env *env, char *line, int i, int j)
{
	free(env->value);
	if (j > i + 1)
		env->value = ft_substr(line, i + 1, j);
	if (!env->value)
		env->value = ft_strdup("");
}

int	ft_export(t_env *env, char *line)
{
	t_env	*tmp;
	t_env	*ptr;
	int		i;
	int		j;

	tmp = env;
	ptr = NULL;
	i = 0;
	if (!env || !line)
		return (1);
	if (!ft_do_export(line))
		return (1);
	if (*line == '\\')
		line = line + 1;
//	printf("line = %s\n", line);
	while (line[i] && line[i] != '=')
		i++;
	j = i + 1;
	while (line[j])
		j++;
	while (tmp && ft_strncmp(tmp->name, line, i + 1) != -61)
		tmp = tmp->next;
	if (!tmp)
		ft_export_new(env, tmp, line);
	else
		ft_export_replace(tmp, line, i, j);
	return (0);
}
