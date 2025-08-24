/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttangcha <ttangcha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:44:38 by ttangcha          #+#    #+#             */
/*   Updated: 2025/08/24 21:02:23 by ttangcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_exit(const char *message)
{
	printf(RED"%s\n"RST, message);
	exit(1);
}

void	*safe_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (ret == NULL)
		error_exit("Error : malloc");
	return (ret);
}

unsigned long	timestamp(t_table *table)
{
	return (get_current_time() - table->start_simulation_time);
}

unsigned long	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		error_exit("Error : gettimeofday");
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_usleep(size_t ms, t_table *table)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < ms)
	{
		if (get_bool(&table->table_mutex, &table->end_simulation))
			break ;
		usleep(50);
	}
	return (0);
}
