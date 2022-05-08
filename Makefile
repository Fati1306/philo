NAME = philo

SRCS = philo.c

OBJS = $(SRCS:.c=.o)

CC = cc

CFLAGS = -Wall -Werror -Wextra

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -I. -fsanitize=address

.c.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o) -I. -fsanitize=address -static-libsan

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

