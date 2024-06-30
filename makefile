.DEFAULT_GOAL := run
INCLUDES := ./alemdar/includes/
CFLAGS = -Wall -Werror -lpng -ljpeg -lm

SOURCE := main.c

run:
	@gcc -I $(INCLUDES) $(SOURCE) $(CFLAGS) && ./a.out

time:
	@if [ -e "a.out" ]; then time ./a.out > /dev/null; fi

test:
	@gcc -g $(SOURCE) $(CFLAGS) -o bin && gdb bin

memcheck:
	@valgrind --tool=memcheck --leak-check=full ./a.out

debug:
	@gcc -g $(SOURCE) $(CFLAGS) -o program

docker:
	@sudo docker build -t alemdar-app . && sudo docker run alemdar-app

clear:
	@rm -f a.out bin program && clear
