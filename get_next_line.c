/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 17:08:05 by marvin            #+#    #+#             */
/*   Updated: 2025/10/16 19:41:35 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	ft_strlen_till_backn(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\n')
		{
			i++;
			break ;
		}
		i++;
	}
	return (i);
}

void	ft_str_shift_right(char *buffer_read, int len)
{
	int	i;

	i = 0;
	while (buffer_read[len])
	{
		buffer_read[i] = buffer_read[len];
		i++;
		len++;
	}
	buffer_read[i] = '\0';
}

int	read_buffersize(int fd, char *buffer)
{
	int	bytes_read;

	if (buffer[0] != '\0')
		return (1); // déjà rempli, pas besoin de lire
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read <= 0) // EOF / error
		return (0);
	buffer[bytes_read] = '\0'; // sinon read a pu ecraser avec du vide une partie du calloc et ca va jumb des ligne pour r
	return (1);
}

int	ft_checker_fd(int fd)
{
	if (fd < 0)
	{
		ft_putstr_fd("Bad file descriptor !\n", 2);
		return (0);
	}
	else if (fd >= OPEN_MAX_FOR_MULTIPLE_FD)
	{
		ft_putstr_fd("number if file descriptors too high !\n", 2);
		return (0);
	}
	else if (BUFFER_SIZE <= 0)
	{
		ft_putstr_fd("Buffer size of 0 !\n", 2);
		return (0);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static char	buffer_read[OPEN_MAX_FOR_MULTIPLE_FD][BUFFER_SIZE + 1]; // deja calloc
	char		*line;
	int			len;
	int			i;

	if (!ft_checker_fd(fd))
		return (NULL);
	if (!read_buffersize(fd, buffer_read[fd]))
		return (NULL);
	len = ft_strlen_till_backn(buffer_read[fd]);
	line = malloc(len + 1);
	i = 0;
	while (i < len)
	{
		line[i] = buffer_read[fd][i];
		i++;
	}
	line[len] = '\0';
	ft_str_shift_right(buffer_read[fd], len);
	return (line);
}

// int	main(void)
// {
// 	int		infile;
// 	char	*line;

// 	infile = open("infile", O_RDONLY);
// 	while (1)
// 	{
// 		line = get_next_line(infile);
// 		if (line)
// 			printf("%s", line);
// 		if (!line)
// 			break ;
// 		free(line);
// 	}
// 	close(infile);
// 	return (0);
// }
