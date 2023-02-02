CC = g++
OBJS = main.o RgbImage.o materiais.o
FLAG = -lglut -lGLU -lGL
BINF = car

prog:		compile
		@echo "Runing"
		./${BINF}
		@echo "End program"


compile:	${OBJS}
		@echo "Compiling"
		${CC} ${OBJS} ${FLAG} -o ${BINF}
		@echo "Compile finished"

clean:
		rm ${OBJS} ${BINF}

zip:
		zip project.zip *

%.o:		%.c
		$(CC) -c $^ $(FLAGS) 
