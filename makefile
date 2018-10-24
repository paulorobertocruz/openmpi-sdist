EXECUTAVEL=teste
MPICC=mpicc

all: ${EXECUTAVEL}

main: teste.c
	${MPICC} -o teste teste.c

clean:
	rm -f ${EXECUTAVEL} sdist.o image_filter.out

image_filter: sdist.o
	gcc -o image_filter.out sdist.o image_filter.c

sdist.o:
	gcc -c sdist.c