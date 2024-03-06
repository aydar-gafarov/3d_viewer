#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QColorDialog>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>
#include <QtOpenGL>
#ifdef __cplusplus
extern "C" {
#endif
#include "s21_parser.h"
#ifdef __cplusplus
}
#endif

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 private:
  double xRot, yRot;
  QPoint mPos;
  void different_projection();
  void paint_points();
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void wheelEvent(QWheelEvent *mo) override;
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

 public:
  GLWidget(QWidget *parent = nullptr);
  double x_move;
  double y_move;
  double z_move;
  double x_rotate;
  double y_rotate;
  double z_rotate;
  double red_point = 255;
  double green_point = 255;
  double blue_point = 255;
  QColor color_point_;
  int flag_ = 0;
  QColor color_line_;
  QColor back_color;
  void open_f(const char *filename);
  data_t data_struct = {};
  double *vertex_array_ = NULL;
  double scale = 0.5;
  int point_size_ = 0;
  int line_size = 0;
  double min_x = -1, min_y = -1, max_x = 1, max_y = 1;
  int var_projection_ = 0;
  int point_type_ = 0;
  int line_type_ = 0;
  int status_ = 1;
  int *facet_array_ = NULL;
};

#endif  // GLWIDGET_H
