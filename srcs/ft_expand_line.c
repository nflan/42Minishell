#include "../include/minishell.h"

char	*ft_noquote_line(char *line)
{
	char	*new;
	int		ij[2];

	ij[0] = -1;
	ij[1] = 0;
	new = NULL;
	if (!line)
		return (NULL);
	if (ft_strlen(line) == ft_strlen_nq(line))
		return (line);
	new = ft_calloc(sizeof(char), ft_strlen_nq(line) + 1);
	if (!new)
		return (NULL);
	while (line[++ij[0]])
	{
		if (line[ij[0]] == '\'')
			while (line[++ij[0]] != '\'')
				new[ij[1]++] = line[ij[0]];
		if (line[ij[0]] == '\"')
			while (line[++ij[0]] != '\"')
				new[ij[1]++] = line[ij[0]];
		if (line[ij[0]] != '\'' && line[ij[0]] != '\"')
			new[ij[1]++] = line[ij[0]];
	}
	return (free(line), new);
}

int	ft_get_length(char *str, int length, int t)
{
	if (str[length] && t == 1)
		while (str[length] && str[length] != '\"' && str[length] != ' ')
			length++;
	else if (str[length] && t == 0)
		while (str[length] && str[length] != '\"' && str[length] != '\''
			&& str[length] != ' ')
			length++;
	else if (str[length] && t == 2)
		while (str[length] && str[length] != '\'' && str[length] != ' ')
			length++;
	return (length);
}

char	*ft_expand_line(char *str, int *i, t_info *info, int t)
{
	char	*tmp[4];
	int		length;

	length = *i;
	tmp[0] = ft_substr(str, 0, *i - 1);
	if (!tmp[0])
		return (NULL);
	length = ft_get_length(str, length, t);
	tmp[3] = ft_substr(str, *i, length - *i);
	if (!tmp[3])
		return (free(tmp[0]), NULL);
	tmp[1] = ft_expanded_value(info, tmp[3]);
	if (!tmp[1])
		return (free(tmp[0]), NULL);
	tmp[2] = ft_substr(str, length, ft_strlen(str));
	if (!tmp[2])
		return (free(tmp[0]), free(tmp[1]), NULL);
	free(str);
	*i = length - 1;
	return (ft_strjoiiin_free(tmp[0], tmp[1], tmp[2], 4));
}
//	printf("tmp[0] = '%s' && tmp[1] = '%s' && tmp[2] = '%s'\n",
//	tmp[0], tmp[1], tmp[2]);

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
			if (t < 2 || hd)
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
