#include "mainwindow.h"

#include "glwidget.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->ortho_button->setChecked(1);
  ui->none_point_radio->setChecked(1);
  ui->solid_line_radio->setChecked(1);
  connect(&timer, &QTimer::timeout, (this), &MainWindow::make_gif);
}

MainWindow::~MainWindow() {
  free_memory();
  delete ui;
}

void MainWindow::make_gif() {
  if (image_count < 50) {
    ui->openGLWidget->setStyleSheet(
        "background-color: " + ui->openGLWidget->back_color.name() + ";");
    image_gif[image_count] = ui->openGLWidget->grab().toImage();
    image_count++;
  } else {
    gif = new QGifImage;
    for (int i = 0; i < image_count; i++) {
      gif->addFrame(image_gif[i], 0);
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "/home", tr("GIF (*.gif)"));
    gif->save(fileName);
    ui->gif_Button->setEnabled(true);
    timer.stop();
    delete gif;
    image_count = 0;
  }
}

void MainWindow::on_gif_Button_clicked() {
  ui->gif_Button->setEnabled(false);
  timer.start(100);
}

void MainWindow::free_memory() {
  if (ui->openGLWidget->facet_array_ != NULL) {
    s21_remove_matrix(&ui->openGLWidget->data_struct.matrix);
    for (int i = 0; i <= (int)ui->openGLWidget->data_struct.count_facets; i++) {
      free(ui->openGLWidget->data_struct.polygon[i].vertex);
    }
    free(ui->openGLWidget->data_struct.polygon);
    free(ui->openGLWidget->facet_array_);
  }
  if (ui->openGLWidget->vertex_array_ != NULL) {
    free(ui->openGLWidget->vertex_array_);
  }
}

void MainWindow::on_ortho_button_clicked() {
  ui->openGLWidget->var_projection_ = 0;
  ui->openGLWidget->update();
}

void MainWindow::on_frustum_button_clicked() {
  ui->openGLWidget->var_projection_ = 1;
  ui->openGLWidget->update();
}

void MainWindow::on_move_x_slider_valueChanged(int value) {
  ui->openGLWidget->x_move -= value;
  double x = ui->openGLWidget->x_move;
  if (x != 0) {
    x = x / 10;
    move_x(&ui->openGLWidget->data_struct.matrix, x);
    make_array_vertex(&ui->openGLWidget->data_struct,
                      ui->openGLWidget->vertex_array_);
  }
  ui->openGLWidget->x_move = value;
  ui->openGLWidget->update();
}

void MainWindow::on_move_y_slider_valueChanged(int value) {
  ui->openGLWidget->y_move -= value;
  double y = ui->openGLWidget->y_move;
  if (y != 0) {
    y = y / 10;
    move_y(&ui->openGLWidget->data_struct.matrix, y);
    make_array_vertex(&ui->openGLWidget->data_struct,
                      ui->openGLWidget->vertex_array_);
  }
  ui->openGLWidget->y_move = value;
  ui->openGLWidget->update();
}

void MainWindow::on_move_z_slider_valueChanged(int value) {
  ui->openGLWidget->z_move -= value;
  double z = ui->openGLWidget->z_move;
  if (z != 0) {
    z = z / 10;
    move_z(&ui->openGLWidget->data_struct.matrix, z);
    make_array_vertex(&ui->openGLWidget->data_struct,
                      ui->openGLWidget->vertex_array_);
  }
  ui->openGLWidget->z_move = value;
  ui->openGLWidget->update();
}

void MainWindow::on_rotate_x_slider_valueChanged(int value) {
  ui->openGLWidget->x_rotate -= value;
  double x = ui->openGLWidget->x_rotate;
  if (x != 0) {
    x = x / 10;
    rotation_by_ox(&ui->openGLWidget->data_struct.matrix, x);
    make_array_vertex(&ui->openGLWidget->data_struct,
                      ui->openGLWidget->vertex_array_);
  }
  ui->openGLWidget->x_rotate = value;
  ui->openGLWidget->update();
}

void MainWindow::on_rotate_y_slider_valueChanged(int value) {
  ui->openGLWidget->y_rotate -= value;
  double y = ui->openGLWidget->y_rotate;
  if (y != 0) {
    y = y / 10;
    rotation_by_oy(&ui->openGLWidget->data_struct.matrix, y);
    make_array_vertex(&ui->openGLWidget->data_struct,
                      ui->openGLWidget->vertex_array_);
  }
  ui->openGLWidget->y_rotate = value;
  ui->openGLWidget->update();
}

void MainWindow::on_rotate_z_slider_valueChanged(int value) {
  ui->openGLWidget->z_rotate -= value;
  double z = ui->openGLWidget->z_rotate;
  if (z != 0) {
    z = z / 10;
    rotation_by_oz(&ui->openGLWidget->data_struct.matrix, z);
    make_array_vertex(&ui->openGLWidget->data_struct,
                      ui->openGLWidget->vertex_array_);
  }
  ui->openGLWidget->z_rotate = value;
  ui->openGLWidget->update();
}

void MainWindow::reset_values() {
  ui->openGLWidget->data_struct.count_facets = 0;
  ui->openGLWidget->data_struct.count_vertex = 0;
  ui->openGLWidget->data_struct.indexes = 0;
  ui->zoom_slider->setValue(10);
  ui->move_x_slider->setValue(0);
  ui->move_y_slider->setValue(0);
  ui->move_z_slider->setValue(0);
  ui->rotate_x_slider->setValue(0);
  ui->rotate_y_slider->setValue(0);
  ui->rotate_z_slider->setValue(0);
  zoom_k = 1.0;
}

void MainWindow::on_choose_file_button_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open File"), "/home", tr("objectFiles (*.obj)"));
  reset_values();
  free_memory();
  ui->openGLWidget->open_f(fileName.toStdString().c_str());
  if (ui->openGLWidget->status_ == 0 && fileName != "") {
    int count_vertexes = ui->openGLWidget->data_struct.count_vertex;
    int edges =
        count_vertexes + ui->openGLWidget->data_struct.count_facets / 2 + 2;
    ui->result_filename_label->setText(fileName);
    ui->result_vertex_label->setText(QString::number(count_vertexes, 'g', 10));
    ui->reselt_edges_label->setText(QString::number(edges, 'g', 10));
  }
}

void MainWindow::on_zoom_slider_valueChanged(int value) {
  zoom_k = ((double)value / 10) / zoom_k;
  if (zoom_k != 0) {
    zoom_function(&ui->openGLWidget->data_struct.matrix, zoom_k * zoom_k);
    make_array_vertex(&ui->openGLWidget->data_struct,
                      ui->openGLWidget->vertex_array_);
  }
  zoom_k = (double)value / 10;
  ui->openGLWidget->update();
}

void MainWindow::on_none_point_radio_clicked() {
  ui->openGLWidget->point_type_ = 0;
  ui->openGLWidget->update();
}

void MainWindow::on_circle_point_radio_clicked() {
  ui->openGLWidget->point_type_ = 1;
  ui->openGLWidget->update();
}

void MainWindow::on_square_point_radio_clicked() {
  ui->openGLWidget->point_type_ = 2;
  ui->openGLWidget->update();
}

void MainWindow::on_point_size_spinBox_valueChanged(int arg1) {
  ui->openGLWidget->point_size_ = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_line_color_Button_clicked() {
  QColor pre_color = QColorDialog::getColor();
  if (pre_color.isValid()) {
    ui->openGLWidget->color_line_ = pre_color;
    ui->openGLWidget->update();
  }
}

void MainWindow::on_color_point_Button_clicked() {
  QColor pre_color = QColorDialog::getColor();
  if (pre_color.isValid()) {
    ui->openGLWidget->color_point_ = pre_color;
    ui->openGLWidget->update();
  }
}

void MainWindow::on_solid_line_radio_clicked() {
  ui->openGLWidget->line_type_ = 1;
  ui->openGLWidget->update();
}

void MainWindow::on_line_size_spinBox_2_valueChanged(int arg1) {
  ui->openGLWidget->line_size = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_dashed_line_radio_clicked() {
  ui->openGLWidget->line_type_ = 2;
  ui->openGLWidget->update();
}

void MainWindow::on_back_color_Button_clicked() {
  QColor pre_color = QColorDialog::getColor();
  if (pre_color.isValid()) {
    ui->openGLWidget->back_color = pre_color;
    ui->openGLWidget->update();
  }
}

void MainWindow::on_screen_Button_clicked() {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save File"), "/home", tr("BMP (*.bmp);; JPEG (*.jpeg)"));
  ui->openGLWidget->setStyleSheet(
      "background-color: " + ui->openGLWidget->back_color.name() + ";");
  QPixmap file = ui->openGLWidget->grab();
  file.save(fileName);
}
