/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-maac <fel-maac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 14:59:37 by fel-maac          #+#    #+#             */
/*   Updated: 2022/05/22 14:59:41 by fel-maac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    print_lock(t_philo *philo, char *status, int id)
{
    long int	elapsed_time;

    elapsed_time = get_time() - philo->o->t0;
    pthread_mutex_lock(&philo->o->print);
    printf("%ldms %d %s\n", elapsed_time, id, status);
	if (philo->o->dead == 0)
        pthread_mutex_unlock(&philo->o->print);
}

void	philo_eating(t_philo *philo, int s)
{
	pthread_mutex_lock(&philo->o->forks[philo->id]);
	print_lock(philo, "has taken fork 1", philo->id + 1);
	pthread_mutex_lock((&philo->o->forks[philo->id + s]));
	print_lock(philo, "has taken fork 2", philo->id + 1);
	print_lock(philo, "is eating", philo->id + 1);
	philo->last_meal = get_time();
	ft_usleep(philo->o->eat * 1000);
	pthread_mutex_unlock(&philo->o->forks[philo->id]);
	pthread_mutex_unlock(&philo->o->forks[philo->id + s]);
}

void    *philo_thread(void *args)
{
    t_philo *philo;
    int     s;

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
        print_lock(philo, "is sleeping", philo->id + 1);
        ft_usleep(philo->o->sleep * 1000);
        print_lock(philo, "is thinking", philo->id + 1);
    }
    return (args);
}

void    *supervisor_thread(void *args)
{
    t_philo 	*philos;
	int 		i;
	long int	current_time;

    philos = (t_philo *)args;
	while (1)
	{
		i = -1;
		while (++i < philos[0].o->nb_ph)
		{
			usleep(100);
			current_time = get_time();
			if (current_time - philos[i].last_meal > philos[0].o->die)
			{
				philos->o->dead = 1;
				print_lock(&philos[i], "died", i + 1);
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

    if (check_if_num(ac, av) == -1 || ac < 5 || ac > 6 || ft_atoi(av[1]) == 0
        || init_ops(&o, ac, av) == -1)
    {
        printf("Error\n");
        return (1);
    }
    philos = (t_philo *) malloc(sizeof(t_philo) * o.nb_ph);
    i = -1;
    while (++i < o.nb_ph)
    {
        philos[i].o = &o;
        philos[i].id = i;
        philos[i].last_meal = o.t0;
        pthread_mutex_init(&o.forks[i], NULL);
        pthread_create(&philos->thread, NULL, philo_thread, &philos[i]);
    }
    pthread_create(&supervisor, NULL, supervisor_thread, philos);
    pthread_join(supervisor, NULL);
    return (0);
}