#include "philo.h"

void print_status(t_philo_status status, t_philo *philo)
{
	unsigned long	ts;
	int				id;
	bool			ended;

	ts = timestamp(philo->table);
	id = philo->philo_id;
	ended = get_bool(&philo->table->table_mutex, &philo->table->end_simulation);
	if (ended && status != DIED)
		return;
	pthread_mutex_lock(&philo->table->print_mutex);
	if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK) && !ended)
		printf("%lu %d has taken a fork\n", ts, id);
	else if (status == EATING && !ended)
		printf("%lu %d is eating\n", ts, id);
	else if (status == SLEEPING && !ended)
		printf("%lu %d is sleeping\n", ts, id);
	else if (status == THINKING && !ended)
		printf("%lu %d is thinking\n", ts, id);
	else if (status == DIED)
		printf("%lu %d died\n", ts, id);
	pthread_mutex_unlock(&philo->table->print_mutex);
}

