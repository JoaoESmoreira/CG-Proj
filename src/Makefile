
CC = g++
OBJS = meu_codigo.o RgbImage.o materiais.o
FLAG = -lglut -lGLU -lGL

prog:	run


run:	${OBJS}
	${CC} ${OBJS} ${FLAG}
	./a.out

clean:
	rm a.out ${OBJS}

zip:
	zip project.zip *

%.o:	%.c
	$(CC) -c $^ $(FLAGS) 
