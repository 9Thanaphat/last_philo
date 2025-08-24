/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttangcha <ttangcha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:02:47 by ttangcha          #+#    #+#             */
/*   Updated: 2025/08/24 22:46:37 by ttangcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define RST		"\033[0m"
#define RED		"\033[1;31m"

typedef pthread_mutex_t	t_mtx;
typedef struct s_philo	t_philo;
typedef struct s_table	t_table;

typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED,
}	t_philo_status;

typedef struct s_fork
{
	int		fork_id;
	t_mtx	fork;
}	t_fork;

typedef struct s_philo
{
	int			philo_id;
	int			meals_count;
	bool		full;
	long		last_meal_time;
	pthread_t	thread;
	t_fork		*first_fork;
	t_fork		*second_fork;
	t_mtx		philo_mtx;
	t_table		*table;
}	t_philo;

typedef struct s_table
{
	int				philo_nbr;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				limit_meal_nbr;
	size_t			start_simulation_time;
	bool			end_simulation;
	bool			all_thread_ready;
	t_mtx			print_mutex;
	t_mtx			table_mutex;
	pthread_t		monitor;
	t_philo			*philos;
	t_fork			*forks;
}	t_table;

//parse.c
void			parse(t_table *table, char **av);

//init.c
void			data_init(t_table *table);

//monitor.c
void			*monitor(void *arg);

//simulation.c
void			simulation_start(t_table *table);

//philo_routine.c
void			thinking(t_philo *p);
void			sleeping(t_philo *p);
void			eating(t_philo *p);

//print.c
void			print_status(t_philo_status status, t_philo *p);

//getter.c
bool			get_bool(t_mtx *mutex, bool *value);
long			get_long(t_mtx *mutex, long *value);
int				get_int(t_mtx *mutex, int *value);

//setter.c
void			set_bool(t_mtx *mutex, bool *dest, bool value);
void			increase_nbr(t_mtx *mutex, int *dest);
void			set_long(t_mtx *mutex, long *dest, long value);

//utils.c
void			error_exit(const char *message);
void			*safe_malloc(size_t bytes);
size_t			get_current_time(void);
int				ft_usleep(size_t ms, t_table *table);
size_t			timestamp(t_table *table);
