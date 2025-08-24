#include "philo.h"

// static void thinking(t_philo *p) //dynamic method
// {
//     print_status(THINKING, p);

//     long now   = get_current_time();
//     long last  = get_long(&p->philo_mtx, &p->last_meal_time);
//     long left  = (long)p->table->time_to_die - (now - last); // เวลาที่เหลือก่อนตาย
//     long need  = (long)p->table->time_to_eat;                // เวลาอย่างน้อยที่ต้องเหลือไว้สำหรับกินรอบหน้า
//     if (left <= need) return;

//     // กัน jitter ของ OS หน่อย (~5% ของ TTD, บีบให้อยู่ 1..6 ms)
//     long guard = p->table->time_to_die / 20;
//     if (guard < 1) guard = 1;
//     if (guard > 6) guard = 6;

//     long slack = left - need - guard;   // เวลาปลอดภัยที่เราจะเอาไปคิดได้
//     if (slack <= 0) return;

//     long t = slack / 2;                 // ใช้แค่ครึ่ง เพื่อลด lockstep/pileup
//     if (t < 1) t = 1;
//     ft_usleep((size_t)t, p->table);
// }

void thinking(t_philo *p)
{
	size_t	t;

	print_status(THINKING, p);
	if (p->table->philo_nbr % 2)
	{
		if (p->table->time_to_eat > 1)
			t = p->table->time_to_eat - 1;
		else
			t = 0;
		ft_usleep(t, p->table);
	}
	else
		ft_usleep(1, p->table);
}

void	sleeping(t_philo *philo)
{
	print_status(SLEEPING, philo);
	ft_usleep(philo->table->time_to_sleep, philo->table);
}

void	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->first_fork->fork);
	print_status(TAKE_FIRST_FORK, philo);
	pthread_mutex_lock(&philo->second_fork->fork);
	print_status(TAKE_SECOND_FORK, philo);
	set_long(&philo->philo_mtx, &philo->last_meal_time, get_current_time());
	increase_nbr(&philo->philo_mtx, &philo->meals_count);
	print_status(EATING, philo);
	ft_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->limit_meal_nbr > 0 && get_int(&philo->philo_mtx, &philo->meals_count) == philo->table->limit_meal_nbr)
		set_bool(&philo->philo_mtx, &philo->full, true);
	pthread_mutex_unlock(&philo->second_fork->fork);
	pthread_mutex_unlock(&philo->first_fork->fork);
}

void	*philo_routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	while (!get_bool(&philo->table->table_mutex, &philo->table->all_thread_ready))
		usleep(200);
	if (philo->table->philo_nbr % 2 == 0)
	{
		if (philo->philo_id % 2 == 0)
			ft_usleep(philo->table->time_to_eat / 2, philo->table);
	}
	else
	{
		if (philo->philo_id % 2 == 0)
			ft_usleep(philo->table->time_to_eat, philo->table);
	}
	while (!get_bool(&philo->table->table_mutex, &philo->table->end_simulation))
	{
		if (get_bool(&philo->philo_mtx, &philo->full))
			break;
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}

void *one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->first_fork->fork);
	print_status(TAKE_FIRST_FORK, philo);
	ft_usleep(philo->table->time_to_die, philo->table);
	print_status(DIED, philo);
	pthread_mutex_unlock(&philo->first_fork->fork);
	return NULL;
}

void	simulation_start(t_table *table)
{
	int	i;

	if (table->philo_nbr == 1)
	{
		table->start_simulation_time = get_current_time();
		pthread_create(&table->philos[0].thread, 0, one_philo, &table->philos[0]);
		pthread_join(table->philos[0].thread, NULL);
		return ;
	}
	table->start_simulation_time = get_current_time();
	i = -1;
	while (++i < table->philo_nbr)
		set_long(&table->philos[i].philo_mtx,
			&table->philos[i].last_meal_time,
			table->start_simulation_time);
	i = -1;
	while (++i < table->philo_nbr)
		pthread_create(&table->philos[i].thread, 0, philo_routine, &table->philos[i]);
	pthread_create(&table->monitor, 0, monitor, table);
	set_bool(&table->table_mutex, &table->all_thread_ready, true);
	i = -1;
	while (++i < table->philo_nbr)
		pthread_join(table->philos[i].thread, NULL);
	pthread_join(table->monitor, NULL);
}
