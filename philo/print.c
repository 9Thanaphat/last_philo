#include "philo.h"

static void	print_message(t_philo_status s, size_t ts, int id)
{
	if (s == TAKE_FIRST_FORK || s == TAKE_SECOND_FORK)
		printf("%zu %d has taken a fork\n", ts, id);
	else if (s == EATING)
		printf("%zu %d is eating\n", ts, id);
	else if (s == SLEEPING)
		printf("%zu %d is sleeping\n", ts, id);
	else if (s == THINKING)
		printf("%zu %d is thinking\n", ts, id);
	else if (s == DIED)
		printf("%zu %d died\n", ts, id);
}

void	print_status(t_philo_status s, t_philo *p)
{
	size_t	ts;
	int		id;

	if (s != DIED && get_bool(&p->table->table_mutex,
			&p->table->end_simulation))
		return ;
	pthread_mutex_lock(&p->table->print_mutex);
	if (s != DIED && get_bool(&p->table->table_mutex,
			&p->table->end_simulation))
	{
		pthread_mutex_unlock(&p->table->print_mutex);
		return ;
	}
	ts = timestamp(p->table);
	id = p->philo_id;
	if (!philo_full(p->table))
		print_message(s, ts, id);
	pthread_mutex_unlock(&p->table->print_mutex);
}
