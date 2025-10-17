/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 17:08:05 by throbert          #+#    #+#             */
/*   Updated: 2025/10/17 14:27:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// GNL V2, cleaner, shorter (handle bonuses)

#include "get_next_line.h"

int	ft_strlen_till_backn(const char *s)
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
	static char	buffer_read[OPEN_MAX_FOR_MULTIPLE_FD][BUFFER_SIZE + 1];
	char		*line;
	int			len;
	int			i;

	if (!ft_checker_fd(fd))
		return (NULL);
	if (!read_of_buffersize(fd, buffer_read[fd]))
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
	ft_str_shift_left(buffer_read[fd], len);
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

int main(void)
{
    int fd1, fd2;
    char *line1;
    char *line2;

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

        if (!line1 && !line2) // plus rien dans aucun fichier
            break;

        if (line1)
        {
            printf("fd1: %s", line1);
            free(line1);
        }
        if (line2)
        {
            printf("fd2: %s", line2);
            free(line2);
        }
    }

    close(fd1);
    close(fd2);
    return (0);
}
