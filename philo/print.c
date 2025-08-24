/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttangcha <ttangcha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:45:14 by ttangcha          #+#    #+#             */
/*   Updated: 2025/08/24 20:50:36 by ttangcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_message(t_philo_status s, unsigned long ts, int id)
{
	if (s == TAKE_FIRST_FORK || s == TAKE_SECOND_FORK)
		printf("%lu %d has taken a fork\n", ts, id);
	else if (s == EATING)
		printf("%lu %d is eating\n", ts, id);
	else if (s == SLEEPING)
		printf("%lu %d is sleeping\n", ts, id);
	else if (s == THINKING)
		printf("%lu %d is thinking\n", ts, id);
	else if (s == DIED)
		printf("%lu %d died\n", ts, id);
}

void	print_status(t_philo_status s, t_philo *p)
{
	bool			ended;
	int				id;
	unsigned long	ts;

	pthread_mutex_lock(&p->table->print_mutex);
	if (s != DIED)
	{
		ended = get_bool(&p->table->table_mutex, &p->table->end_simulation);
		if (ended)
		{
			pthread_mutex_unlock(&p->table->print_mutex);
			return ;
		}
	}
	ts = timestamp(p->table);
	id = p->philo_id;
	print_message(s, ts, id);
	pthread_mutex_unlock(&p->table->print_mutex);
}
