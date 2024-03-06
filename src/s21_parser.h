#ifndef S21_PARSER_H
#define S21_PARSER_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *strdup(const char *);

typedef struct matrix_3d {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

typedef struct facets {
  int *vertex;
  int number_of_vertex_in_facets;
} polygon_t;

typedef struct data {
  unsigned int count_vertex;
  unsigned int count_facets;
  unsigned int indexes;
  matrix_t matrix;
  polygon_t *polygon;
} data_t;

int parse_file(const char *filename, data_t *data_s);
int counting_vertexes_and_facets(const char *filename, data_t *data_struct);
int facet_parse(const char *filename, data_t *data_struct);
int find_number_vertex_in_facets(data_t *data_str, char *buff, char *str,
                                 int j);
void create_array_vertex(data_t *data_str, char *buff, char *str, int j);
void find_max(double *x_1, double *x_2, double *y_1, double *y_2,
              data_t *data_struct);
void make_array_vertex(data_t *data_str, double *vertex_array);
void make_array_index_facets(data_t *data_str, int *index_array);
void mem_free(data_t *data_str);
// Matrix
int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);
void print_matrix(matrix_t *matrix);

void move_x(matrix_t *matrix_struct, double a);
void move_y(matrix_t *matrix_struct, double a);
void move_z(matrix_t *matrix_struct, double a);

void rotation_by_ox(matrix_t *matrix_struct, double angle);
void rotation_by_oy(matrix_t *matrix_struct, double angle);
void rotation_by_oz(matrix_t *matrix_struct, double angle);

void zoom_function(matrix_t *matrix_struct, double zoom_k);
#endif
