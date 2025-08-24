/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttangcha <ttangcha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:44:46 by ttangcha          #+#    #+#             */
/*   Updated: 2025/08/24 19:44:46 by ttangcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void assign_fork(t_philo *philo, t_fork *forks, int i)
{
	t_fork	*left;
	t_fork	*right;

	left = &forks[i];
	right = &forks[(i + 1) % philo->table->philo_nbr];
	if (philo->philo_id % 2)
	{
		philo->first_fork  = left;
		philo->second_fork = right;
	}
	else
	{
		philo->first_fork  = right;
		philo->second_fork = left;
	}
}

void	philo_init(t_table *table)
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
