#include "s21_parser.h"

void zoom_function(matrix_t *matrix_struct, double zoom_k) {
  if (zoom_k != 0) {
    for (int i = 1; i < matrix_struct->rows; i++) {
      for (int j = 0; j < 3; j++) {
        matrix_struct->matrix[i][j] *= zoom_k;
      }
    }
  }
}

void mem_free(data_t *data_str) {
  if (data_str->matrix.matrix) {
    s21_remove_matrix(&data_str->matrix);
  }
  if (data_str->polygon) {
    for (int i = 0; i <= (int)data_str->count_facets; i++) {
      free(data_str->polygon[i].vertex);
    }
    free(data_str->polygon);
  }
  // }
  // if (ui->openGLWidget->vertex_array_ != NULL) {
}

void rotation_by_ox(matrix_t *matrix_struct, double angle) {
  for (int i = 1; i < matrix_struct->rows; i++) {
    double temp_y = matrix_struct->matrix[i][1];
    double temp_z = matrix_struct->matrix[i][2];
    matrix_struct->matrix[i][1] = cos(angle) * temp_y - sin(angle) * temp_z;
    matrix_struct->matrix[i][2] = sin(angle) * temp_y + cos(angle) * temp_z;
  }
}

void rotation_by_oy(matrix_t *matrix_struct, double angle) {
  for (int i = 1; i < matrix_struct->rows; i++) {
    double temp_x = matrix_struct->matrix[i][0];
    double temp_z = matrix_struct->matrix[i][2];
    matrix_struct->matrix[i][0] = cos(angle) * temp_x + sin(angle) * temp_z;
    matrix_struct->matrix[i][2] = -sin(angle) * temp_x + cos(angle) * temp_z;
  }
}

void rotation_by_oz(matrix_t *matrix_struct, double angle) {
  for (int i = 1; i < matrix_struct->rows; i++) {
    double temp_x = matrix_struct->matrix[i][0];
    double temp_y = matrix_struct->matrix[i][1];
    matrix_struct->matrix[i][0] = cos(angle) * temp_x - sin(angle) * temp_y;
    matrix_struct->matrix[i][1] = sin(angle) * temp_x + cos(angle) * temp_y;
  }
}

void move_x(matrix_t *matrix_struct, double a) {
  for (int i = 1; i < matrix_struct->rows; i++)
    matrix_struct->matrix[i][0] += a;
}

void move_y(matrix_t *matrix_struct, double a) {
  for (int i = 1; i < matrix_struct->rows; i++)
    matrix_struct->matrix[i][1] += a;
}

void move_z(matrix_t *matrix_struct, double a) {
  for (int i = 1; i < matrix_struct->rows; i++)
    matrix_struct->matrix[i][2] += a;
}

void make_array_index_facets(data_t *data_str, int *index_array) {
  int b = 0;
  int first_facet = 0;
  for (unsigned int i = 1; i < data_str->count_facets + 1; i++) {
    first_facet = 0;
    for (int j = 0; j < data_str->polygon[i].number_of_vertex_in_facets; j++) {
      if (data_str->polygon[i].vertex[j] < 0)
        data_str->polygon[i].vertex[j] += data_str->count_vertex + 1;
      if (j == 0) {
        first_facet = data_str->polygon[i].vertex[j];
        index_array[b++] = first_facet;
      } else {
        index_array[b++] = data_str->polygon[i].vertex[j];
        index_array[b++] = data_str->polygon[i].vertex[j];
      }
    }

    index_array[b++] = first_facet;
  }
}

void find_max(double *x_1, double *x_2, double *y_1, double *y_2,
              data_t *data_struct) {
  double max_x = -2;
  double min_x = 2;
  double max_y = -2;
  double min_y = 2;
  double x = 0;
  double y = 0;
  for (int i = 0; i < data_struct->matrix.rows; i++) {
    x = data_struct->matrix.matrix[i][0];
    y = data_struct->matrix.matrix[i][1];
    if (x > max_x) max_x = x;
    if (x < min_x) min_x = x;
    if (y > max_y) max_y = y;
    if (y < min_y) min_y = y;
  }
  *x_1 = max_x;
  *x_2 = min_x;
  *y_1 = max_y;
  *y_2 = min_y;
}

int parse_file(const char *filename, data_t *data_s) {
  int status = 0;
  status = counting_vertexes_and_facets(filename, data_s);
  if (status == 0) {
    status = facet_parse(filename, data_s);
  }
  return status;
}

int counting_vertexes_and_facets(const char *filename, data_t *data_struct) {
  int status = 0;
  char buffer[1024] = {0};
  FILE *ptr_file = NULL;
  if ((ptr_file = fopen(filename, "r")) != NULL) {
    while (fgets(buffer, 1024, ptr_file) != NULL) {
      if (buffer[0] == 'v' && buffer[1] == ' ') {
        data_struct->count_vertex++;
      } else if (buffer[0] == 'f' && buffer[1] == ' ') {
        data_struct->count_facets++;
      }
    }
    fclose(ptr_file);
  } else
    status = 1;
  return status;
}

void make_array_vertex(data_t *data_str, double *vertex_array) {
  int b = 0;
  for (int i = 0; i < data_str->matrix.rows; i++) {
    for (int j = 0; j < data_str->matrix.columns; j++) {
      vertex_array[b++] = data_str->matrix.matrix[i][j];
    }
  }
}

int facet_parse(const char *filename, data_t *data_struct) {
  int status = 0;
  int columns = 3;
  char buffer[2048] = {0};
  int rows = data_struct->count_vertex;
  status = s21_create_matrix(rows + 1, columns, &data_struct->matrix);
  data_struct->polygon =
      calloc(data_struct->count_facets + 1, sizeof(polygon_t));
  FILE *ptr_file = NULL;
  if ((ptr_file = fopen(filename, "r")) != NULL && status == 0) {
    int i = 1;
    int j = 1;
    char *str = NULL;
    int cnt_facets_in_line = 0;
    while (fgets(buffer, 2048, ptr_file) != NULL) {
      if (buffer[0] == 'v' && buffer[1] == ' ') {
        sscanf(buffer, "v %lf %lf %lf", &data_struct->matrix.matrix[i][0],
               &data_struct->matrix.matrix[i][1],
               &data_struct->matrix.matrix[i][2]);
        i++;
      } else if (buffer[0] == 'f' && buffer[1] == ' ') {
        char *asd = strdup(buffer);
        cnt_facets_in_line =
            find_number_vertex_in_facets(data_struct, buffer, str, j);
        data_struct->indexes += cnt_facets_in_line;
        data_struct->polygon[j].number_of_vertex_in_facets = cnt_facets_in_line;
        create_array_vertex(data_struct, asd, str, j);
        j++;
        free(asd);
      }
    }
    fclose(ptr_file);
  }
  return status;
}

int find_number_vertex_in_facets(data_t *data_str, char *buff, char *str,
                                 int j) {
  int cnt_in_line = 0;
  str = strtok(buff, " ");
  while ((str != NULL)) {
    str = strtok(NULL, " ");
    if (str != NULL && str[0] != '\n') cnt_in_line++;
  }
  data_str->polygon[j].number_of_vertex_in_facets = cnt_in_line;
  return cnt_in_line;
}

void create_array_vertex(data_t *data_str, char *buff, char *str, int j) {
  data_str->polygon[j].vertex = calloc(
      data_str->polygon[j].number_of_vertex_in_facets, sizeof(polygon_t));
  str = strtok(buff, " ");
  int b = 0;
  while (str != NULL) {
    str = strtok(NULL, " ");
    if (str != NULL) {
      data_str->polygon[j].vertex[b] = atoi(str);
      b++;
    }
  }
}
int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int status = 0;
  if (columns <= 0 || rows <= 0) {
    status = 1;
  } else {
    result->rows = rows;
    result->columns = columns;
    result->matrix = (double **)calloc(rows, sizeof(double *));
    if (result->matrix != NULL) {
      for (int i = 0; i < rows; i++) {
        result->matrix[i] = (double *)calloc(columns, sizeof(double));
      }
    } else
      status = 1;
  }
  return status;
}

void print_matrix(matrix_t *matrix) {
  for (int i = 0; i < matrix->rows; i++) {
    for (int j = 0; j < matrix->columns; j++) {
      printf("%f ", matrix->matrix[i][j]);
    }
    printf("\n");
  }
}

void s21_remove_matrix(matrix_t *A) {
  for (int i = 0; i < A->rows; i++) {
    free(A->matrix[i]);
  }
  free(A->matrix);
  A->columns = 0;
  A->rows = 0;
}
