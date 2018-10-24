#ifndef _sdist_h
#define _sdist_h

//utils
void print_matrix(int **m, int r, int c);

//vetores
int ** get_2d_matrix(int a, int b);
int ** get_transpose_2d_matrix(int **m, int r, int c);
int dot_product(int *a, int *b, int size);
int ** multply_matrix(int **A, int a_rows, int a_cols, int **B, int b_rows, int b_cols);

//processamento de imagem
int image_kernel_filter(int *image, int *kernel, int length);

#endif