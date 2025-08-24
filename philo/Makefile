NAME    = philo

CC      = cc
CFLAG   =
LDFLAG  = -pthread

SOURCE  = main.c \
			parse.c \
			init.c \
			getter.c \
			setter.c \
			monitor.c \
			simulation.c \
			print.c \
			utils.c \

HEADER  = .

OFILE   = $(SOURCE:.c=.o)

all: $(NAME)

$(NAME): $(OFILE)
	$(CC) $(CFLAG) $(OFILE) -o $(NAME) $(LDFLAG)

%.o: %.c
	$(CC) $(CFLAG) -I $(HEADER) -c $< -o $@

clean:
	rm -fv $(OFILE)

fclean: clean
	rm -fv $(NAME)

re: fclean all

.PHONY: all clean fclean re
