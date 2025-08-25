/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttangcha <ttangcha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:44:46 by ttangcha          #+#    #+#             */
/*   Updated: 2025/08/25 08:47:44 by ttangcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void assign_fork(t_philo *p, t_fork *forks, int i)
{
	t_fork	*l;
	t_fork	*r;


	l = &forks[i];
	r = &forks[(i + 1) % p->table->philo_nbr];
	if (l->fork_id < r->fork_id)
	{
		p->first_fork  = l;
		p->second_fork = r;
	}
	else
	{
		p->first_fork  = r;
		p->second_fork = l;
	}
}


static void	philo_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->philo_nbr)
	{
		philo = &table->philos[i];
		philo->philo_id = i + 1;
		philo->full = false;
		philo->meals_count = 0;
		pthread_mutex_init(&philo->philo_mtx, NULL);
		philo->table = table;
		philo->thread = 0;
		assign_fork(philo, table->forks, i);
	}
}

void	data_init(t_table *table)
{
	int		i;

	table->end_simulation = false;
	table->all_thread_ready = false;
	table->philos = safe_malloc(sizeof(t_philo) * table->philo_nbr);
	pthread_mutex_init(&table->table_mutex, NULL);
	pthread_mutex_init(&table->print_mutex, NULL);
	table->forks = safe_malloc(sizeof(t_fork) * table->philo_nbr);
	i = -1;
	while (++i < table->philo_nbr)
	{
		pthread_mutex_init(&table->forks[i].fork, NULL);
		table->forks[i].fork_id = i;
	}
	philo_init(table);
}
