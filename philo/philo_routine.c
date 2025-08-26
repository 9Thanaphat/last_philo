/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttangcha <ttangcha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:01:36 by ttangcha          #+#    #+#             */
/*   Updated: 2025/08/26 08:57:35 by ttangcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	thinking(t_philo *p)
{
	long	t;

	t = 0;
	print_status(THINKING, p);
	if (p->table->time_to_eat >= p->table->time_to_sleep)
	{
		if (p->table->philo_nbr % 2 != 0)
			t = (long)p->table->time_to_eat * 2 - (long)p->table->time_to_sleep;
		else
			t = (long)p->table->time_to_eat - (long)p->table->time_to_sleep;
	}
	else
	{
		if (p->table->philo_nbr % 2 != 0)
			t = (long)p->table->time_to_eat * 2 - (long)p->table->time_to_sleep;
		else
			t = 0;
	}
	if (t < 0)
		t = 0;
	if (t > 0)
		ft_usleep((size_t)t, p->table);
}

void	sleeping(t_philo *p)
{
	print_status(SLEEPING, p);
	ft_usleep(p->table->time_to_sleep, p->table);
}

void	eating(t_philo *p)
{
	pthread_mutex_lock(&p->first_fork->fork);
	print_status(TAKE_FIRST_FORK, p);
	pthread_mutex_lock(&p->second_fork->fork);
	print_status(TAKE_SECOND_FORK, p);
	set_long(&p->philo_mtx, &p->last_meal_time, get_current_time());
	increase_nbr(&p->philo_mtx, &p->meals_count);
	print_status(EATING, p);
	ft_usleep(p->table->time_to_eat, p->table);
	if (p->table->limit_meal_nbr > 0 && get_int(&p->philo_mtx,
			&p->meals_count) == p->table->limit_meal_nbr)
		set_bool(&p->philo_mtx, &p->full, true);
	pthread_mutex_unlock(&p->second_fork->fork);
	pthread_mutex_unlock(&p->first_fork->fork);
}
