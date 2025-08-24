/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttangcha <ttangcha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:01:36 by ttangcha          #+#    #+#             */
/*   Updated: 2025/08/24 21:01:36 by ttangcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static long	think_time_ms(t_philo *p)
{
	long	left;
	long	guard;
	long	slack;
	long	t;

	left = (long)p->table->time_to_die
		- (get_current_time() - get_long(&p->philo_mtx, &p->last_meal_time));
	if (left <= (long)p->table->time_to_eat)
		return (0);
	guard = (long)p->table->time_to_die / 20;
	if (guard < 1)
		guard = 1;
	if (guard > 6)
		guard = 6;
	slack = left - (long)p->table->time_to_eat - guard;
	if (slack <= 0)
		return (0);
	t = slack / 2;
	if (t < 1)
		t = 1;
	return (t);
}

void	thinking(t_philo *p)
{
	long	t;

	print_status(THINKING, p);
	t = think_time_ms(p);
	if (t > 0)
		ft_usleep((size_t)t, p->table);
}

// void thinking(t_philo *p)
// {
// 	size_t	t;

// 	print_status(THINKING, p);
// 	if (p->table->philo_nbr % 2)
// 	{
// 		if (p->table->time_to_eat > 1)
// 			t = p->table->time_to_eat - 1;
// 		else
// 			t = 0;
// 		ft_usleep(t, p->table);
// 	}
// 	else
// 		ft_usleep(1, p->table);
// }

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
