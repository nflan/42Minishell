#include "../include/minishell.h"

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
