EXECUTAVEL=main.out
MPICC=mpicc

all: ${EXECUTAVEL}

main.out: main.c sdist.o
	${MPICC} -o ${EXECUTAVEL} sdist.o main.c

clean:
	rm -f ${EXECUTAVEL} sdist.o image_filter.out

image_filter: sdist.o
	gcc -o image_filter.out sdist.o image_filter.c

sdist.o: sdist.c
	gcc -c sdist.c
