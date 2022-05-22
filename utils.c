/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-maac <fel-maac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 14:59:25 by fel-maac          #+#    #+#             */
/*   Updated: 2022/05/22 15:00:13 by fel-maac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int				i;
    unsigned long	num;
    int				sign;
	
	i = 0;
    num = 0;
    sign = 1;
    if (str[i] == '-' || str[i] == '+')
        if (str[i++] == '-')
            sign *= -1;
    while (str[i])
    {
        if (!(str[i] >= '0' && str[i] <= '9'))
            return (num * sign);
        num = (num * 10) + str[i++] - '0';
        if (num > 9223372036854775807)
        {
            if (sign < 0)
                return (0);
            return (-1);
        }
    }
    return (num * sign);
}

int check_if_num(int ac, char **args)
{
    int i;

    while (--ac > 0)
    {
        i = -1;
        while(args[ac][++i])
        {
            if (!(args[ac][i] >= '0' && args[ac][i] <= '9'))
            {
                if (i == 0 && (args[ac][0] == '-' || args[ac][0] == '+'))
                    i++;
                else
                    return (-1);
            }
        }
    }
    return (0);
}

long int    get_time(void)
{
    struct timeval  t;
    long int        m_sec;
    long int        m_usec;

    gettimeofday(&t, NULL);
    m_sec = t.tv_sec * 1000;
    m_usec = t.tv_usec / 1000;
    return (m_sec + m_usec);
}

void    ft_usleep(long int goal)
{
    long int    time;

    time = get_time() * 1000;
    while (get_time() * 1000 <= time + goal)
        usleep(100);
}

int init_ops(t_ops *o, int ac, char **av)
{
	int i;

	i = -1;
	o->nb_ph = ft_atoi(av[1]);
	o->die = ft_atoi(av[2]);
	o->eat = ft_atoi(av[3]);
	o->sleep = ft_atoi(av[4]);
	o->nb_eat = -1;
	o->forks = NULL;
	if (pthread_mutex_init(&o->print, NULL) != 0)
		return (-1);
	o->forks = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * o->nb_ph);
	o->t0 = get_time();
	o->dead = 0;
	if (ac == 6)
	{
		o->nb_eat = ft_atoi(av[5]);
		o->meals = (int *) malloc(sizeof(int) * o->nb_ph);
		while (++i < o->nb_ph)
			o->meals[i] = o->nb_eat;
	}
	if (o->forks == NULL || o->meals == NULL)
		return (-1);
	return (0);
}
