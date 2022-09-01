/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-maac <fel-maac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 14:59:37 by fel-maac          #+#    #+#             */
/*   Updated: 2022/09/01 12:51:00 by fel-maac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// need to add error checking for usleep & gettimeofday

#include "philo.h"

void	print_lock(t_philo *philo, char *status, int id)
{
	long int	elapsed_time;

	elapsed_time = get_time() - philo->o->t0;
	pthread_mutex_lock(&philo->o->print);
	printf("%ldms %d %s\n", elapsed_time, id + 1, status);
	// add mutex for dead here
	if (philo->o->dead == 0)
		pthread_mutex_unlock(&philo->o->print);
}

void	philo_eating(t_philo *philo, int s)
{
	pthread_mutex_lock(&philo->o->forks[philo->id]);
	print_lock(philo, "has taken 1st fork", philo->id);
	pthread_mutex_lock((&philo->o->forks[philo->id + s]));
	print_lock(philo, "has taken 2nd fork", philo->id);
	print_lock(philo, "is eating", philo->id);
	printf("test \t %d\n", philo->o->meals[0]);
	if (philo->o->nb_eat != -1)
		philo->o->meals[philo->id]--;
	// add last meal mutex here
	philo->last_meal = get_time();
	ft_usleep(philo->o->eat * 1000);
	pthread_mutex_unlock(&philo->o->forks[philo->id]);
	pthread_mutex_unlock(&philo->o->forks[philo->id + s]);
}

void	*philo_thread(void *args)
{
	t_philo	*philo;
	int		s;

	philo = (t_philo *)args;
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		if (philo->id == philo->o->nb_ph - 1)
			s = -philo->id;
		else
			s = 1;
		philo_eating(philo, s);
		print_lock(philo, "is sleeping", philo->id);
		ft_usleep(philo->o->sleep * 1000);
		print_lock(philo, "is thinking", philo->id);
	}
	return (args);
}

int	check_meals(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos[0].o->nb_ph && philos[0].o->meals[i] <= 0)
		i++;
	if (i == philos[0].o->nb_ph)
		return (0);
	return (1);
}

void	*supervisor_thread(void *args)
{
	t_philo		*philos;
	int			i;
	long int	current_time;

	philos = (t_philo *)args;
	while (1)
	{
		i = -1;
		while (++i < philos[0].o->nb_ph)
		{
			usleep(100);
			// removed usleep bc check meal might take time
			if (philos[0].o->nb_eat != -1)
			{
				if (check_meals(philos) == 0)
					return (args);
			}
			current_time = get_time();
			// add last meal mutex here
			if (current_time - philos[i].last_meal > philos[0].o->die)
			{
				// add dead mutex here
				philos->o->dead = 1;
				print_lock(&philos[i], "died", i);
				return (args);
			}
		}
	}
    return (args);
}

int main(int ac, char *av[])
{
    int         i;
    t_ops       o;
    t_philo     *philos;
    pthread_t   supervisor;

    if (check_if_num(ac, av) == -1 || ac < 5 || ac > 6
		|| ft_atoi(av[1]) == 0 || init_ops(&o, ac, av) == -1)
    {
        printf("Error\n");
        return (1);
    }
	philos = NULL;
    philos = (t_philo *) malloc(sizeof(t_philo) * o.nb_ph);
	if (philos == NULL)
	{
        printf("Error\n");
		return (1);
	}
    i = -1;
    while (++i < o.nb_ph)
    {
        philos[i].o = &o;
        philos[i].id = i;
        philos[i].last_meal = o.t0;
        if (pthread_mutex_init(&o.forks[i], NULL) != 0
			|| pthread_create(&philos->thread, NULL, philo_thread, &philos[i]) != 0)
		{
			 printf("Error\n");
        	return (1);
		}
    }
    if (pthread_create(&supervisor, NULL, supervisor_thread, philos) != 0
		|| pthread_join(supervisor, NULL) != 0)
	{
        printf("Error\n");
		return (1);
	}
    return (0);
}