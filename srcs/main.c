/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 11:39:37 by nflan             #+#    #+#             */
/*   Updated: 2022/05/12 18:02:47 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_keep_history(char *str)
{
	int	i;

	i = 0;
	if (str)
		while (*str && (*str == ' ' || *str == '\n'))
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
	signal(SIGINT, &ft_signal);
}

int	ft_exit(char *str)
{
	if (str)
		while (*str && *str == ' ')
			str++;
	if (!ft_strncmp(str, "exit", 5) || !ft_strncmp(str, "exit ", 5))
		return (0);
	return (1);
}

char	*ft_cmd_line(char *str)
{
	char	*cmd;
	int		i;
	int		y;

	i = -1;
	cmd = NULL;
	if (str)
		while (*str && *str == ' ')
			str++;
	if (str)
		while (str[++i])
			if ((str[i] == '|' && str[i + 1] == '|')
				|| (str[i] == '&' && str[i + 1] == '&'))
				break ;
	cmd = ft_calloc(sizeof(char *), i);
	if (!cmd)
		return (NULL);
	y = -1;
	while (++y < i)
		cmd[y] = str[y];
	return (cmd);
}

int	ft_fill_cmd(t_info *info, int i)
{
	char	*cmd_line;
	t_cmd	*tmp;

	tmp = NULL;
	cmd_line = ft_cmd_line(info->rdline);
	for (int y = 0; y < 3; y++)
	{
		info->cmd[i]->line = cmd_line;
		if (!info->cmd[0]->line)
			return (1);
		info->cmd[i]->fdin = 0;
		info->cmd[i]->fdout = 1;
		info->cmd[i]->index = 0;
		info->cmd[i]->next = tmp;
		info->cmd[i]->prev = NULL;
		tmp = info->cmd[i];
	}
	return (0);
}

int	ft_init_cmd(t_info *info)
{
	int	i;
	
	i = -1;
	info->cmd = NULL;
	info->cmd = ft_calloc(sizeof(t_cmd *), info->nbdiff + 1);
	if (!info->cmd)
		return (1);
	while (++i <= info->nbdiff)
	{
		info->cmd[i] = ft_calloc(sizeof(t_cmd), 1);
		if (!info->cmd[i])
			return (1);
		if (ft_fill_cmd(info, i))
			return (1);
	}
	return (0);
}

void	ft_print_cmd(t_info *info)
{
	int	i;

	i = 0;
	if (info->cmd)
	{
		while (info->cmd[i]->next)
		{
			printf("info->cmd->line = %s\n", info->cmd[i]->line);
			printf("info->cmd->fdin = %d\n", info->cmd[i]->fdin);
			printf("info->cmd->fdout = %d\n", info->cmd[i]->fdout);
			printf("info->cmd->index = %d\n", info->cmd[i]->index);
			info->cmd[i] = info->cmd[i]->next;
		}
	}
}

int	ft_nb_andor(char *str)
{
	int	count;
	int	i;

	count = 0;
	i = -1;
	if (str)
		while (str[++i + count])
			if ((str[i + count] == '|' && str[i + count + 1] == '|')
				|| (str[i + count] == '&' && str[i + count + 1] == '&'))
					count++;
	return (count);
}

int	ft_init_info(t_info *info)
{
	info->nbdiff = ft_nb_andor(info->rdline);
	if (ft_init_cmd(info))
		return (1);
	ft_print_cmd(info);
	return (0);
}

int	main(int ac, char **av)
{
	static char	*line;
	t_info		info;

	(void) av;
	if (ac > 1)
		return (ft_putstr_fd("Too much arguments\n", 2), 1);
	signal(SIGINT, &ft_signal);
	while (1)
	{
		info.rdline = NULL;
		info.rdline = readline("Minishell$>");
		printf("line = %s\n", info.rdline);
		if (!info.rdline || !ft_exit(info.rdline))
			break ;
		else if (ft_keep_history(info.rdline))
			add_history(info.rdline);
		if (ft_init_info(&info))
			return (1);
		free(line);
	}
	return (0);
}
