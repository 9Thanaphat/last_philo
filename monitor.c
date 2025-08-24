#include "philo.h"

static bool philo_died(t_philo *philo)
{
	long	last_meal_time;
	long	current_time;
	long	time_to_die;

	current_time = get_current_time();
	time_to_die = philo->table->time_to_die;
	if (get_bool(&philo->philo_mtx, &philo->full))
		return false;
	last_meal_time = get_long(&philo->philo_mtx, &philo->last_meal_time);
	if (current_time - last_meal_time >= time_to_die)
		return true;
	return false;
}

static bool philo_died_at(t_philo *p, long now_ms)
{
	long
	last;
	if (get_bool(&p->philo_mtx, &p->full))
		return false;
	last = get_long(&p->philo_mtx, &p->last_meal_time);
	return (now_ms - last) >= p->table->time_to_die;
}
static bool philo_full(t_table *table)
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
			break;
		}
	}
	if (all_full)
	{
		set_bool(&table->table_mutex, &table->end_simulation, true);
			return (true);
	}
	return (false);
}

void *monitor(void *arg)
{
	int				i;
	long			now;
	unsigned long	ts;
	t_table *t;

	t = arg;
	while (!get_bool(&t->table_mutex, &t->all_thread_ready))
		usleep(200);
	while (!get_bool(&t->table_mutex, &t->end_simulation))
	{
		now = get_current_time();
		i = -1;
		while (++i < t->philo_nbr)
		{
			if (philo_died_at(&t->philos[i], now))
			{
				set_bool(&t->table_mutex, &t->end_simulation, true);
				pthread_mutex_lock(&t->print_mutex);
				ts = now - t->start_simulation_time;
				printf("%lu %d died\n", ts, t->philos[i].philo_id);
				pthread_mutex_unlock(&t->print_mutex);
				return NULL;
			}
		}
		if (t->limit_meal_nbr > 0 && philo_full(t))
			return (NULL);
		usleep(100);
	}
	return NULL;
}
