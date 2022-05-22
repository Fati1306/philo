/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-maac <fel-maac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 14:59:34 by fel-maac          #+#    #+#             */
/*   Updated: 2022/05/22 14:59:35 by fel-maac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

int	        ft_atoi(const char *str);
int         check_if_num(int ac, char **args);
int	        ft_strncmp(const char *s1, const char *s2, int n);
long int    get_time(void);
void        ft_usleep(long int goal);

typedef struct  s_ops
{
    int             nb_ph;
    int             die;
    int             eat;
    int             sleep;
    int             nb_eat;
    pthread_mutex_t print;
    pthread_mutex_t *forks;
    long int        t0;
    int             dead;
    int             *meals;
}   t_ops;

typedef struct s_philo
{
    int         id;
    pthread_t   thread;
    long int    last_meal;
    t_ops       *o;
}   t_philo;

#endif
