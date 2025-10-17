/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 17:08:05 by throbert          #+#    #+#             */
/*   Updated: 2025/10/17 19:08:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// GNL V2, cleaner, shorter (handle bonuses)
// Make re BUFFER_SIZE=1

#include "get_next_line.h"

int	ft_strlen_till_backn(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\n')
			return (i + 1);
		i++;
	}
	return (i);
}

// recrit depuis apres \n par dessus le debut.   puis ecrase le \n restant pour raccourcir la str. \0

// buffersize = 2 et que la ligne fait 2 lettre + \n
// le \n est pas read
// mais strlen est quand meme a 2
// donc buffer_read[len] = \0 car a ete terminated dans read_of_buffersize
// donc va return un chaine vide ("" == str[0] = \0)
void	ft_str_shift_left(char *buffer_read, int len)
{
	int	i;

	i = 0;
	while (buffer_read[len]) // si que \n rentre pas dedans
	{
		buffer_read[i] = buffer_read[len];
		i++;
		len++;
	}
	buffer_read[i] = '\0'; // fini la string plus tot et osef de ce qui vient apres  ||  SI QUE \n l ecrase
}
// Hello\n123\0
// 
// 123\0lo\n\0  ===> la str buffer s arrete au premier \0 trouver. apres c est consider comme du garbage
//
// prochaine iteration la string sera 123 et le strjoin ecrira par dessus l ancien reste.

//  si deux \n se suivent
//
// len = 1.
// buffer[1] = \n  donc va mettre \n sur \n

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

int	read_of_buffersize(int fd, char *buffer)
{
	int	bytes_read;

	if (buffer[0] != '\0') // sinon vide car tab[] remplit de \0
		return (1); // déjà rempli, pas besoin de lire
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read <= 0) // EOF / error
		return (0);
	buffer[bytes_read] = '\0';
	return (1);
}

char	*process_and_join(char *line, char *buf, int len)
{
	char	*temp;
	char	*new_line;
	int		i;

	temp = malloc(len + 1);
	if (!temp)
		return (free(line), NULL);
	i = -1;
	while (++i < len)
		temp[i] = buf[i];
	temp[len] = '\0';
	if (!line)
		new_line = ft_strdup(temp);
	else
	{
		new_line = ft_strjoin(line, temp);
		free(line);
	}
	free(temp);
	return (new_line);
}

char	*get_next_line(int fd)
{
	static char	buffer_read[OPEN_MAX_FOR_MULTIPLE_FD][BUFFER_SIZE + 1];
	char		*line;
	int			len_till_backn;
	int			finish_by_newline;

	if (!ft_checker_fd(fd))
		return (NULL);
	line = NULL;
	while (1)
	{
		if (!read_of_buffersize(fd, buffer_read[fd]))
			return (line);
		len_till_backn = ft_strlen_till_backn(buffer_read[fd]);
		if (len_till_backn == 0)
			return (line);
		finish_by_newline = (buffer_read[fd][len_till_backn - 1] == '\n'); // verif si le len till \n a stop pour \n ou juste car fin de str. 
		line = process_and_join(line, buffer_read[fd], len_till_backn);
		if (!line)
			return (NULL);
		ft_str_shift_left(buffer_read[fd], len_till_backn);
		if (finish_by_newline) // si pas stop pour fin de str continu.
			return (line);
	}
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

int	main(void)
{
	char	*line1;
	char	*line2;
	int		fd1;
	int		fd2;

	fd1 = open("file1", O_RDONLY);
	fd2 = open("file2", O_RDONLY);
	if (fd1 < 0 || fd2 < 0)
	{
		perror("open");
		return (1);
	}
	while (1)
	{
		line1 = get_next_line(fd1);
		line2 = get_next_line(fd2);
		if (!line1 && !line2)
			break ;
		if (line1)
		{
			printf("fd1: %s", line1);
			if (line1[strlen(line1) - 1] != '\n')
				printf("\n");
			free(line1);
		}
		if (line2)
		{
			printf("fd2: %s", line2);
			if (line2[strlen(line2) - 1] != '\n')
				printf("\n");
			free(line2);
		}
	}
	close(fd1);
	close(fd2);
	return (0);
}
