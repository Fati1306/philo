/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-maac <fel-maac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 14:59:25 by fel-maac          #+#    #+#             */
/*   Updated: 2022/09/07 12:05:40 by fel-maac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int				i;
	unsigned long	num;

	i = 0;
	num = 0;
	while (str[i])
		num = (num * 10) + str[i++] - '0';
	if (num > 2147483647)
		return (-1);
	return (num);
}

int	check_if_num(int ac, char **args)
{
	int	i;

	while (--ac > 0)
	{
		i = -1;
		while (args[ac][++i])
		{
			if (!(args[ac][i] >= '0' && args[ac][i] <= '9'))
				return (-1);
		}
	}
	return (0);
}

long int	get_time(void)
{
	struct timeval	t;
	long int		m_sec;
	long int		m_usec;

	gettimeofday(&t, NULL);
	m_sec = t.tv_sec * 1000;
	m_usec = t.tv_usec / 1000;
	return (m_sec + m_usec);
}

void	ft_usleep(long int goal)
{
	long int	time;

	time = get_time() * 1000;
	while (get_time() * 1000 <= time + goal)
		usleep(100);
}

int	init_ops(t_ops *o, int ac, char **av)
{
	int	i;

	i = -1;
	o->nb_ph = ft_atoi(av[1]);
	o->die = ft_atoi(av[2]);
	o->eat = ft_atoi(av[3]);
	o->sleep = ft_atoi(av[4]);
	if (o->nb_ph == -1 || o->die == -1 || o->eat == -1 || o->sleep == -1)
		return (-1);
	o->nb_eat = -1;
	if (pthread_mutex_init(&o->print_m, NULL) != 0)
		return (-1);
	o->forks_m = NULL;
	o->forks_m = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * o->nb_ph);
	o->l_meal_m = NULL;
	o->l_meal_m = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * o->nb_ph);	
	o->meals_m = NULL;
	o->t0 = get_time();
	o->dead = 0;
	if (ac == 6)
	{
		o->nb_eat = ft_atoi(av[5]);
		o->meals_m = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * o->nb_eat);
		o->meals = NULL;
		o->meals = (int *) malloc(sizeof(int) * o->nb_ph);
		while (++i < o->nb_ph)
			o->meals[i] = o->nb_eat;
	}
	if (o->forks_m == NULL || o->meals == NULL || o->l_meal_m == NULL
		|| (o->nb_eat != -1 && o->meals_m == NULL))
		return (-1);
	return (0);
}
