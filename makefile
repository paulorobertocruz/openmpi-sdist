EXECUTAVEL=teste
MPICC=mpicc

all: ${EXECUTAVEL}

main: teste.c
	${MPICC} -o teste teste.c

clean:
	rm -f ${EXECUTAVEL}