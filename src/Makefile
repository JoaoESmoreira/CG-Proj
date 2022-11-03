
CC = g++
OBJS = meu_codigo.o
FLAG = -lglut -lGLU -lGL

prog:	run


run:	${OBJS}
	${CC} ${OBJS} ${FLAG}
	./a.out

clean:
	rm a.out ${OBJS}

%.o:	%.c
	$(CC) -c $^ $(FLAGS) 
