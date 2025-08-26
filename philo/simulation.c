/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttangcha <ttangcha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:44:31 by ttangcha          #+#    #+#             */
/*   Updated: 2025/08/26 09:14:44 by ttangcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*philo_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	while (!get_bool(&p->table->table_mutex, &p->table->all_thread_ready))
		usleep(200);
	if (p->table->philo_nbr % 2 == 0)
	{
		if (p->philo_id % 2 == 0)
			ft_usleep(p->table->time_to_eat / 2, p->table);
	}
	else
	{
		if (p->philo_id % 2 == 0)
			ft_usleep(p->table->time_to_eat, p->table);
	}
	while (!get_bool(&p->table->table_mutex, &p->table->end_simulation))
	{
		if (get_bool(&p->philo_mtx, &p->full))
			break ;
		eating(p);
		sleeping(p);
		thinking(p);
	}
	return (NULL);
}

static void	*one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->first_fork->fork);
	print_status(TAKE_FIRST_FORK, philo);
	ft_usleep(philo->table->time_to_die, philo->table);
	print_status(DIED, philo);
	pthread_mutex_unlock(&philo->first_fork->fork);
	return (NULL);
}

void	simulation_start(t_table *t)
{
	int	i;

	if (t->philo_nbr == 1)
	{
		t->start_simulation_time = get_current_time();
		pthread_create(&t->philos[0].thread, 0, one_philo, &t->philos[0]);
		pthread_join(t->philos[0].thread, NULL);
		return ;
	}
	t->start_simulation_time = get_current_time();
	i = -1;
	while (++i < t->philo_nbr)
		set_long(&t->philos[i].philo_mtx,
			&t->philos[i].last_meal_time,
			t->start_simulation_time);
	i = -1;
	while (++i < t->philo_nbr)
		pthread_create(&t->philos[i].thread, 0, philo_routine, &t->philos[i]);
	pthread_create(&t->monitor, 0, monitor, t);
	set_bool(&t->table_mutex, &t->all_thread_ready, true);
	i = -1;
	while (++i < t->philo_nbr)
		pthread_join(t->philos[i].thread, NULL);
	pthread_join(t->monitor, NULL);
}
