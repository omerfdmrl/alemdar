
CC = cc
COMPILER = gcc
FLAGS = 
LIBRARIES = -lm -lpng -ljpeg -lcjson `sdl2-config --cflags` `sdl2-config --libs`
RM = rm -f

SOURCE = main.c
OUTPUT = program
NAME = libican.a
TNAME = bin

TPATH=test/
UPATH=test/src/
OPATH=build/objs/
RPATH=build/results/

CFILES = $(wildcard src/*.c)
OFILES = $(patsubst src/%.c, $(OPATH)%.o, $(CFILES))
TFILES = $(wildcard $(UPATH)*.c)

all: $(NAME)

$(NAME): $(OFILES)
	ar rcs $(RPATH)$(NAME) $<
	ranlib $(RPATH)$(NAME)

$(OPATH)%.o: src/%.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	@if [ -f $(OPATH)$(NAME) ]; then $(RM) $(OPATH)$(NAME); fi

fclean:
	@if [ -f $(OPATH)$(NAME) ]; then $(RM) $(OPATH)$(NAME); fi
	@if [ -n "$(OFILES)" ]; then $(RM) $(OFILES); fi
	@if [ -f $(TNAME) ]; then $(RM) $(TNAME); fi
	$(RM) -r $(RPATH)$(OUTPUT)
	$(RM) -r $(RPATH)$(TNAME)
	$(RM) -r $(RPATH)$(NAME)


$(OUTPUT):
	$(COMPILER) $(FLAGS) $(SOURCE) $(CFILES) $(LIBRARIES) -o $(RPATH)$(OUTPUT)

run: $(OUTPUT)
	./$(RPATH)$(OUTPUT)

gtest:
	$(COMPILER) -g $(FLAGS) $(SOURCE) $(CFILES) $(LIBRARIES) -o $(RPATH)$(TNAME) && gdb $(RPATH)$(TNAME)

utest:
	$(COMPILER) $(FLAGS) $(TPATH)$(SOURCE) $(CFILES) $(LIBRARIES) $(TFILES) -o $(RPATH)$(TNAME) && ./$(RPATH)$(TNAME)

memcheck: $(OUTPUT)
	valgrind --tool=memcheck --leak-check=full -s ./$(RPATH)$(OUTPUT)

.PHONY: all clean fclean run test memcheck
.DEFAULT_GOAL = run