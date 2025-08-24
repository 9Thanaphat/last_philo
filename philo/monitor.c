/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttangcha <ttangcha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:44:57 by ttangcha          #+#    #+#             */
/*   Updated: 2025/08/24 22:47:52 by ttangcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	philo_died_at(t_philo *p, long now_ms)
{
	long	last;

	if (get_bool(&p->philo_mtx, &p->full))
		return (false);
	last = get_long(&p->philo_mtx, &p->last_meal_time);
	return ((now_ms - last) >= p->table->time_to_die);
}

static bool	philo_full(t_table *table)
{
	bool	all_full;
	int		i;

	all_full = true;
	i = -1;
	while (++i < table->philo_nbr)
	{
		if (!get_bool(&table->philos[i].philo_mtx, &table->philos[i].full))
		{
			all_full = false;
			break ;
		}
	}
	if (all_full)
	{
		set_bool(&table->table_mutex, &table->end_simulation, true);
		return (true);
	}
	return (false);
}

static void	wait_until_ready(t_table *t)
{
	while (!get_bool(&t->table_mutex, &t->all_thread_ready))
		usleep(200);
}

static bool	detect_death(t_table *t, long now)
{
	int	i;

	i = -1;
	while (++i < t->philo_nbr)
	{
		if (philo_died_at(&t->philos[i], now))
		{
			set_bool(&t->table_mutex, &t->end_simulation, true);
			print_status(DIED, &t->philos[i]);
			return (true);
		}
	}
	return (false);
}

void	*monitor(void *arg)
{
	t_table	*t;
	long	now;

	t = (t_table *)arg;
	wait_until_ready(t);
	while (!get_bool(&t->table_mutex, &t->end_simulation))
	{
		now = get_current_time();
		if (detect_death(t, now))
			return (NULL);
		if (t->limit_meal_nbr > 0 && philo_full(t))
			return (NULL);
		usleep(50);
	}
	return (NULL);
}
