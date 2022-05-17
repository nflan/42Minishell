/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 11:39:37 by nflan             #+#    #+#             */
/*   Updated: 2022/05/17 11:42:02 by nflan            ###   ########.fr       */
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

/*void	ft_cmdadd_back(t_cmd **acmd, t_cmd *new)
{
	t_cmd	*tmp;

	tmp = NULL;
	tmp = *acmd;
	if (acmd && new)
	{
		if (*acmd == NULL)
			*acmd = new;
		else
		{
			while (tmp->next)
				tmp = tmp->next;
			new->prev = tmp;
			tmp->next = new;
		}
	}
}*/

char	*ft_onecmd(t_info *info, int i)
{
	char	*cmd_line;
	char	*cmd;
	int		y;

	cmd = NULL;
	y = 0;
	cmd_line = ft_cmd_line(info->rdline);
	if (!cmd_line)
		return (NULL);
	if (i)
	{
		y = -1;
		while (i && cmd_line[++y])
			if (cmd_line[y - 1] == '|')
				i--;
	}
	while (cmd_line[y + i] && cmd_line[y + i] != '|')
		i++;
	printf("i = %d\n", i);
	cmd = ft_calloc(sizeof(char), i);
	if (!cmd)
		return (NULL);
	i = -1;
	y -= 1;
	while (cmd_line[++y] && cmd_line[++i] != '|')
		cmd[i] = cmd_line[y];
	return (cmd);
}

/*int	ft_count_pipe(t_info *info)
{
	int	count;

	count = 0;

	return (count);
}*/

/*t_cmd	*ft_cmdnew(t_info *info, char *cmd)
{
	t_cmd	*new;

	new = ft_calloc(sizeof(t_cmd), 1);
	if (!new)
		return (NULL);
	new->cmd = cmd;
	new->fdin = 0;
	new->fdout = 1;
	return (new);
}*/

void	ft_print_cmd(t_cmd	*cmd)
{
	printf("cmd->line = %s\n", cmd->cmd);
	printf("cmd->fdin = %d\n", cmd->fdin);
	printf("cmd->fdout = %d\n", cmd->fdout);
}

t_cmd	*ft_cmdnew(char *cmd, t_tree *ptr)
{
	t_cmd	*tmp;

	if (!cmd)
		return (NULL);
	tmp = NULL;
	tmp = ft_calloc(sizeof(t_cmd), 1);
	if (!tmp)
		return (NULL);
	tmp->cmd = ft_strdup(cmd);
	tmp->fdin = 0;
	tmp->fdout = 1;
	tmp->tree = NULL;
	if (ptr)
		tmp->tree = ptr;
	return (tmp);
}

void	ft_lstadd_tree(t_tree **alst, t_tree *new, int i)
{
	t_tree	*tmp;

	tmp = NULL;
	tmp = *alst;
	if (alst && new)
	{
		if (*alst == NULL)
			*alst = new;
		else if (i == 2)
		{
			while (tmp->right)
				tmp = tmp->right;
			tmp->right = new;
		}
		else if (i == 1)
		{
			while (tmp->right)
				tmp = tmp->right;
			tmp->left = new;
		}
	}
}

t_tree	*ft_treenew(char *str, t_tree *ptr)
{
	t_tree	*new;

	new = NULL;
	new = ft_calloc(sizeof(t_tree), 1);
	if (!new)
		return (NULL);
	new->cmd = ft_cmdnew(str, ptr);
	new->right = NULL;
	new->left = NULL;
	return (new);
}

t_tree	*ft_fill_tree(t_info *info)
{
	t_tree	*ptr;
	t_tree	*new;
	t_tree	*tmpcmd;
	char	**tab;
	int		i;

	i = 0;
	tmpcmd = NULL;
	ptr = NULL;
	new = NULL;
	tab = ft_split(info->rdline, ';');
	if (!tab)
		return (NULL);
//	for (int y = 0; tab[y]; y++)
//		printf("tab[%d] = %s\n", y, tab[y]);
	while (tab[i])
	{
		ptr = ft_treenew(tab[i], ptr);
		if (!ptr)
			return (NULL);
		if (i % 2)
		{
			if (i != 1)
			{
				ft_lstadd_tree(&new, ptr, 2);
				ptr = new->right;
			}
			else
				new = ptr;
			ft_lstadd_tree(&new, tmpcmd, 1);
		}
		else
			tmpcmd = ptr;
		i++;
		if (!tab[i])
			ft_lstadd_tree(&new, tmpcmd, 2);
	}
	ft_free_split(tab, i);
//	printf("tmp = %s\n", tmp->left->cmd->cmd);
	return (new);
}

int	ft_init_tree(t_info *info)
{
//	int	i = 0;

	info->status = 0;
	info->tree = ft_fill_tree(info);
	if (!info->tree)
			return (1);
/*		while (info->tree->right)
	{
		printf("racine %d = %s\n", i, info->tree->cmd->cmd);
		printf("left %d = %s\n", i, info->tree->left->cmd->cmd);
		printf("left->cmd apres %s %d = %s\n", info->tree->cmd->cmd, i, info->tree->cmd->tree->cmd->cmd);
		printf("right %d = %s\n", i, info->tree->right->cmd->cmd);
		i++;
		info->tree = info->tree->right;
	}
*/	return (0);
}

/*void	ft_print_tree(t_info *info)
{
	int	i;
	t_tree	*tree;

	i = 0;
	tree = info->tree;
	if (tree)
	{
		while (tree->left)
		{
			tree = tree->left;
			if (!tree->print)
				ft_print_cmd(tree->cmd);
			tree->print = 1;
		}
		while (tree->parent)
		{
			if (tree->right && !tree->right->print)
			{
				tree = tree->right;
				while (tree->left)
				{
					tree = tree->left;
					if (!tree->print)
						ft_print_cmd(tree->cmd);
					tree->print = 1;
				}
			}
			tree = tree->parent;
		}
	}
}*/

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
//	info->nbdiff = ft_nb_andor(info->rdline);
	info->tree = NULL;
	if (ft_init_tree(info))
		return (1);
	info->status = 0;
//	ft_print_tree(info);
	return (0);
}

void	ft_free_branch(t_tree *branch)
{
	free(branch->cmd->cmd);
	free(branch->cmd);
	free(branch);
	branch = NULL;
}

void	ft_free_tree(t_info *info, t_tree *tofree)
{
	while (info->tree->right)
	{
		while (tofree->right)
		{
			tofree = tofree->right;
			if (tofree->left)
				ft_free_branch(tofree->left);
			if (tofree->right)
				ft_free_branch(tofree->right);
			tofree = info->tree;
		}
	}
	while (info->tree->left)
	{
		while (tofree->left)
		{
			tofree = tofree->left;
			if (tofree->left)
				ft_free_branch(tofree->left);
			if (tofree->right)
				ft_free_branch(tofree->right);
			tofree = info->tree;
		}
	}
}

void	ft_free_all(t_info *info)
{
	t_tree	*tofree;

	tofree = NULL;
	ft_free_tree(info, tofree);
	ft_free_branch(info->tree);
	info->tree = NULL;
	free(info->rdline);
}

void	ft_do_it(t_info *info)
{
	if (!ft_strncmp(info->tree->cmd->cmd, "pwd", 3))
		if (ft_pwd())
			printf("oscour pwd\n");
}

int	main(int ac, char **av)
{
	t_info		info;
	int			sig;

	(void) av;
	sig = 0;
	if (ac > 1)
		return (ft_putstr_fd("Too much arguments\n", 2), 1);
	signal(SIGINT, &ft_signal);
	while (1)
	{
		info.rdline = NULL;
		info.rdline = readline("Minishell$>");
	//	printf("line = %s\n", info.rdline);
		if (!info.rdline || !ft_exit(info.rdline))
			break ;
		else if (ft_keep_history(info.rdline))
			add_history(info.rdline);
		if (ft_init_info(&info))
			return (1);
		ft_do_it(&info);
		sig = info.status;
		ft_free_all(&info);
	}
	rl_clear_history();
	return (sig);
}
