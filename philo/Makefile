NAME    = philo

CC      = cc
CFLAG   = -Wall -Wextra -Werror -pthread

SOURCE  = main.c \
			parse.c \
			init.c \
			getter.c \
			setter.c \
			monitor.c \
			philo_routine.c \
			simulation.c \
			print.c \
			utils.c \

HEADER  = .

OFILE   = $(SOURCE:.c=.o)

all: $(NAME)

$(NAME): $(OFILE)
	$(CC) $(CFLAG) $(OFILE) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAG) -I $(HEADER) -c $< -o $@

clean:
	rm -fv $(OFILE)

fclean: clean
	rm -fv $(NAME)

re: fclean all

.PHONY: all clean fclean re
