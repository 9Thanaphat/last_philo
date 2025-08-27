#include "philo.h"

static long	base_think_time(t_philo *p)
{
	long	t;

	if (p->table->philo_nbr % 2 != 0)
		t = (long)p->table->time_to_eat * 2
			- (long)p->table->time_to_sleep;
	else if (p->table->time_to_eat >= p->table->time_to_sleep)
		t = (long)p->table->time_to_eat
			- (long)p->table->time_to_sleep;
	else
		t = 0;
	if (t < 0)
		t = 0;
	return (t);
}

static long	cap_think_to_safe(t_philo *p, long t)
{
	long	now;
	long	last_meal;
	long	buffer;
	long	safe;

	now = get_current_time();
	last_meal = now - get_long(&p->philo_mtx, &p->last_meal_time);
	buffer = (long)p->table->time_to_die / 20;
	if (buffer < 1)
		buffer = 1;
	if (buffer > 6)
		buffer = 6;
	safe = (long)p->table->time_to_die - last_meal
		- (long)p->table->time_to_eat - buffer;
	if (safe < 0)
		safe = 0;
	if (t > safe)
		t = safe;
	if (t < 0)
		t = 0;
	return (t);
}

void	thinking(t_philo *p)
{
	long	t;

	print_status(THINKING, p);
	t = base_think_time(p);
	t = cap_think_to_safe(p, t);
	if (t > 0)
		ft_usleep((size_t)t, p->table);
}

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
	if (p->table->limit_meal_nbr > 0 && get_int(&p->philo_mtx,
			&p->meals_count) == p->table->limit_meal_nbr)
		set_bool(&p->philo_mtx, &p->full, true);
	ft_usleep(p->table->time_to_eat, p->table);
	pthread_mutex_unlock(&p->second_fork->fork);
	pthread_mutex_unlock(&p->first_fork->fork);
}
