/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:11:41 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/26 12:06:00 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_noquote_line(char *line)
{
	char	*new;
	int		ij[2];

	ij[0] = -1;
	ij[1] = 0;
	new = NULL;
	new = ft_calloc(sizeof(char), ft_strlen_nq(line) + 1);
	if (!new)
		return (free(line), NULL);
	if (!ft_strncmp(line, "\"\"", 3))
		return (free(line), new);
	while (line[++ij[0]])
	{
		if (line[ij[0]] == '\'')
			while (line[++ij[0]] && line[ij[0]] != '\'')
				new[ij[1]++] = line[ij[0]];
		if (line[ij[0]] == '\"')
			while (line[++ij[0]] && line[ij[0]] != '\"')
				new[ij[1]++] = line[ij[0]];
		if (line[ij[0]] && line[ij[0]] != '\'' && line[ij[0]] != '\"')
			new[ij[1]++] = line[ij[0]];
	}
	return (free(line), new);
}

int	ft_get_length(char *str, int i, int t)
{
	int	length;

	length = i;
	if (!t && (str[length] == '\'' || str[length] == '\"'))
		return (0);
	if (str[length] == '?' || ft_isdigit(str[length]))
		return (1);
	if (!ft_isalpha(str[length]))
		return (2);
	if (str[length] && t == 1)
		while (str[length] && (ft_isalpha(str[length])
				|| ft_isdigit(str[length]) || str[length] == '_'))
			length++;
	else if (str[length] && t == 0)
	{
		while (str[length] && (ft_isalpha(str[length])
				|| ft_isdigit(str[length]) || str[length] == '_'))
			length++;
	}
	else if (str[length] && t == 2)
		while (str[length] && str[length] != '\'' && str[length] != ' ')
			length++;
	length -= i;
	return (length);
}

//En arrivant, str est la ligne dans laquelle on va faire l'expand
//i est la position va donner la position du caractere apres le $
//t est le type, 0 = no quote, 1 = double quote, 2 = simple quote
//tmp[0] -> va conserver le debut de str, avant le $ qui doit etre expand
//tmp[1] -> va devenir l'expand du $, vide si ni ? ni une var d'environnement
//tmp[2] -> va conserver la suite de str, apres la valeur a expand
//tmp[3] -> va contenir le nom de la valeur a rechercher dans l'environnement
//Par exemple :
//str = haha"$?"$USERoui
//PREMIERE FOIS QU'ON VA DANS LA FONCTION :
//str = haha"$?"$USERoui
//i = 6
//t = 1
//tmp[0] = haha"
//tmp[1] = 'status code' (on va dire 0)
//tmp[2] = "$USERoui
//tmp[3] = ?
//A LA SORTIE, ON RENVOI LE JOIN DE haha"0"$USERoui
//
//DEUXIEME FOIS QU'ON VA DANS LA FONCTION :
//str = haha"0"$USERoui
//i = 8 (en fonction du status code. ici, 0 donc fait une taille de 1)
//t = 0
//tmp[0] = haha"0"
//tmp[1] = nflan
//tmp[2] = oui
//tmp[3] = USER
//A LA SORTIE, haha"0"nflanoui
//Plus de dollars, on ne rentre plus dans la fonction
char	*ft_expand_line(char *str, int *i, t_info *info, int t)
{
	char	*tmp[4];
	int		length;

	tmp[0] = ft_substr(str, 0, *i - 1);
	if (!tmp[0])
		return (free(str), NULL);
	length = ft_get_length(str, *i, t);
	tmp[3] = ft_substr(str, *i, length);
	if (!tmp[3])
		return (free(str), free(tmp[0]), NULL);
	tmp[1] = ft_expanded_value(info, tmp[3]);
	if (!tmp[1])
		return (free(str), free(tmp[0]), NULL);
	tmp[2] = ft_substr(str, length + *i, ft_strlen(str) + ft_strlen(tmp[1]));
	if (!tmp[2])
		return (free(str), free(tmp[0]), free(tmp[1]), NULL);
	free(str);
	*i = *i + ft_strlen(tmp[1]) - 1;
	return (ft_strjoiiin_free(tmp[0], tmp[1], tmp[2], 4));
}

int	ft_check_exp(char *str, int i, int t, int hd)
{
	if (str[i] && (!t || hd || (t == 1 && str[i] != '\'' && str[i] != '\"'))
		&& (ft_isdigit(str[i]) || ft_isalpha(str[i]) || str[i] == '_'
			|| str[i] == '\"' || str[i] == '\'' || str[i] == '?'))
		return (1);
	return (0);
}

char	*ft_expand_l(char *str, t_info *info, int hd)
{
	int		i;
	int		t;

	i = 0;
	t = 0;
	while (str[i])
	{
		ft_type(str[i], &t);
		if (str[i] == '$')
		{
			i++;
			if (ft_check_exp(str, i, t, hd))
			{
				str = ft_expand_line(str, &i, info, t);
				if (!str)
					return (NULL);
				if ((size_t)i > ft_strlen(str))
					break ;
			}
		}
		else
			i++;
	}
	return (str);
}
