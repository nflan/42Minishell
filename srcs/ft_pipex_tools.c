/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 15:13:19 by nflan             #+#    #+#             */
/*   Updated: 2022/06/06 10:22:54 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*int	ft_fdout_me(t_info *info, t_cmd *cmd)
{
	if (!(ft_strncmp(cmd->cmd, "here_doc", 9)))
		cmd->fdout = open(cmd->av[g->end + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		cmd->fdout = open(cmd->av[g->end + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->fdout < 0)
		return (-1);
	return (0);
}*/

/*int	ft_here(t_global *g, int ret)
{
	char	*buf;

	buf = NULL;
	while (ret > 0)
	{
		ft_print(buf, '\n', g->end - 2);
		buf = get_next_line(0);
		if (!buf)
			return (1);
		if (ft_strncmp(buf, g->av[2], ft_strlen(g->av[2])))
			write(g->fdin, buf, ft_strlen(buf));
		else if (ft_strlen(buf) - 1 == ft_strlen(g->av[2]))
			ret = 0;
		free(buf);
		buf = NULL;
	}
	close(g->fdin);
	g->fdin = open("/tmp/.tmp", O_RDONLY);
	g->end++;
	if (g->fdin < 0)
		return (ft_error(1, g));
	return (0);
}

int	close_pipex_heredoc(t_global *g)
{
	close(g->pdes[1]);
	close(g->fdin);
	unlink("/tmp/.tmp");
	return (0);
}

int	ft_do_heredoc(t_global *g)
{
	ft_here(g, 1);
	if (!g->cmd)
		ft_error(3, g);
	else
	{
		g->child = fork();
		if ((int) g->child == -1)
			return (ft_error(2, g));
		else if ((int) g->child == 0)
		{
			dup2(g->fdin, STDIN_FILENO);
			dup2(g->pdes[1], STDOUT_FILENO);
			close(g->pdes[0]);
			execve(g->cmd[0], g->cmd, g->envp);
			return (ft_error(4, g));
		}
	}
	return (0);
}

int	ft_pipex_heredoc(t_global *g)
{
	if (pipe(g->pdes) == -1)
		return (ft_error(5, g));
	g->cmd = ft_command(g);
	g->fdin = open("/tmp/.tmp", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (g->fdin < 0)
		ft_error(1, g);
	else
		ft_do_heredoc(g);
	return (close_pipex_heredoc(g));
}*/

char	**ft_env_to_tab(t_env *env)
{
	t_env	*tmp;
	char	**tab;
	int		i;

	tab = NULL;
	tmp = env;
	i = 0;
	if (!env)
		return (NULL);
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	tab = ft_calloc(sizeof(char *), i + 1);
	i = 0;
	tmp = env;
	while (tmp)
	{
		tab[i] = ft_strjoiiin(tmp->name, "=", tmp->value);
		i++;
		tmp = tmp->next;
	}
	return (tab);
}
