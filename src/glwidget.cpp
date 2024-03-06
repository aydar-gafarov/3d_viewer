#include "glwidget.h"

GLWidget::GLWidget(QWidget* parent) : QOpenGLWidget{parent} {
  color_point_ = Qt::white;
  color_line_ = Qt::white;
  back_color = Qt::black;
}
void GLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
}
void GLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}
void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(back_color.redF(), back_color.greenF(), back_color.blueF(), 0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslated(0, 0, 0);
  glRotated(xRot, 1, 0, 0);
  glRotated(yRot, 0, 1, 0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glScaled(scale, scale, scale);
  different_projection();
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_DOUBLE, 0, vertex_array_);
  glLineWidth(line_size);
  if (line_type_ == 2) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0x0F0F);
  } else if (line_type_ == 1) {
    glDisable(GL_LINE_STIPPLE);
  }
  glColor3d(color_line_.redF(), color_line_.greenF(), color_line_.blueF());
  glDrawElements(GL_LINES, data_struct.indexes * 2, GL_UNSIGNED_INT,
                 facet_array_);

  glDisableClientState(GL_VERTEX_ARRAY);
  if (point_type_ > 0) paint_points();
}

void GLWidget::paint_points() {
  glPointSize(point_size_);
  if (point_type_ == 1) {
    glEnable(GL_POINT_SMOOTH);
  } else if (point_type_ == 2) {
    glDisable(GL_POINT_SMOOTH);
  }
  glColor3d(color_point_.redF(), color_point_.greenF(), color_point_.blueF());
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_DOUBLE, 0, vertex_array_);
  glDrawArrays(GL_POINTS, 1, data_struct.count_vertex);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void GLWidget::different_projection() {
  if (var_projection_ == 0) {
    glOrtho(min_x, max_x, min_y, max_y, -1000, 1000);
  } else if (var_projection_ == 1) {
    glFrustum(min_x, max_x, min_y, max_y, -1000, 1000);
  }
}

void GLWidget::wheelEvent(QWheelEvent* event) {
  QPoint numDegrees = event->angleDelta();
  if (numDegrees.y() < 0) scale = scale / 1.05;
  if (numDegrees.y() > 0) scale = scale * 1.05;
  update();
}
void GLWidget::mousePressEvent(QMouseEvent* mo) { mPos = mo->pos(); }
void GLWidget::mouseMoveEvent(QMouseEvent* mo) {
  xRot += 1 / M_PI * (mo->pos().y() - mPos.y());
  yRot += 1 / M_PI * (mo->pos().x() - mPos.x());
  mPos = mo->pos();
  update();
}

void GLWidget::open_f(const char* filename) {
  status_ = parse_file(filename, &data_struct);
  if (status_ == 0) {
    vertex_array_ = (double*)malloc(sizeof(double) * data_struct.matrix.rows *
                                    data_struct.matrix.columns);
    facet_array_ = (int*)malloc(sizeof(int) * data_struct.indexes * 2);
    make_array_vertex(&data_struct, vertex_array_);
    make_array_index_facets(&data_struct, facet_array_);
    find_max(&max_x, &min_x, &max_y, &min_y, &data_struct);
  }
}
