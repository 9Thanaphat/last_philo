#include "philo.h"

int	ft_atoi(const char *arg)
{
	int	i;
	int	num;
	int	neg;

	i = 0;
	num = 0;
	neg = 1;
	while (arg[i] == 32 || (arg[i] >= 9 && arg[i] <= 13))
		i++;
	if (arg[i] == '-' || arg[i] == '+')
	{
		if (arg[i] == '-')
			neg = -1;
		i++;
	}
	while (arg[i])
	{
		if (arg[i] >= '0' && arg[i] <= '9')
			num = (num * 10) + (arg[i] - 48);
		else
			break ;
		i++;
	}
	return (num * neg);
}

bool	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (true);
	else
		return (false);
}

bool	check_valid(char *num)
{
	int	i;

	i = 0;
	while (num[i])
	{
		if (!(((num[0] == '+' || num[0] == '-') && ft_isdigit(num[1]))
				|| ft_isdigit(num[i])))
			return (false);
		i++;
	}
	return (true);
}

void	parse(t_table *table, char **av)
{
	int	limit_meal;

	if (!check_valid(av[1]) || !check_valid(av[2])
	|| !check_valid(av[3]) || !check_valid(av[4]))
		error_exit("input error, use number only");
	if (av[5] && !check_valid(av[5]))
		error_exit("input error, use number only");
	table->philo_nbr = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (table->philo_nbr < 1)
		error_exit("philo nbr must more than zero.");
	if (table->time_to_die < 60 || table->time_to_eat < 60
		|| table->time_to_sleep < 60)
		error_exit("use time stamp more than 60ms");
	limit_meal = -1;
	if (av[5])
	{
		limit_meal = ft_atoi(av[5]);
		if (limit_meal < 1)
			error_exit("limit meal nbr must more than zero.");
	}
	table->limit_meal_nbr = limit_meal;
}
