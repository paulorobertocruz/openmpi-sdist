imagem_filter: sdist.o
    gcc -o imagem_filter.out imagem_filter.c

sdist.o:
    gcc -c sdist.c