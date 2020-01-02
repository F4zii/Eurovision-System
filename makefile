OBJS = eurovision_main.o eurovision.o state.o judge.o vote.o libmtm.a
EXEC = main_prog
COMP_FLAGS = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG

$(EXEC): $(OBJS)
	gcc $(COMP_FLAGS) $(OBJS) -o $(EXEC)
eurovision.o: eurovision.c
	gcc -c $(COMP_FLAGS) eurovision.c
state.o: state.c
	gcc -c $(COMP_FLAGS) state.c
judge.o: judge.c
	gcc -c $(COMP_FLAGS) judge.c
vote.o: vote.c
	gcc -c $(COMP_FLAGS) vote.c
eurovision_main.o: eurovision_main.c
	gcc -c $(COMP_FLAGS) eurovision_main.c