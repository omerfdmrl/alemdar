.DEFAULT_GOAL := run
INCLUDES := ./alemdar/includes/
CFLAGS := -Wall -Werror -I$(INCLUDES) `sdl2-config --cflags`
LDFLAGS := `sdl2-config --libs` -lpng -ljpeg -lm

SOURCE := main.c

run:
	@gcc $(SOURCE) $(CFLAGS) $(LDFLAGS) -o program && ./program

time:
	@if [ -e "program" ]; then time ./program > /dev/null; fi

test:
	@gcc -g $(SOURCE) $(CFLAGS) $(LDFLAGS) -o bin && gdb bin

memcheck:
	@valgrind --tool=memcheck --leak-check=full ./program

debug:
	@gcc -g $(SOURCE) $(CFLAGS) $(LDFLAGS) -o program

docker:
	@sudo docker build -t alemdar-app . && sudo docker run alemdar-app

clear:
	@rm -f program bin && clear
