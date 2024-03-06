#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>

#include "QtgifImage/qgifimage.h"
#include "glwidget.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "s21_parser.h"
#ifdef __cplusplus
}
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  double zoom_k = 1.0;

 private slots:

  void on_ortho_button_clicked();

  void on_frustum_button_clicked();

  void on_move_x_slider_valueChanged(int value);

  void on_move_y_slider_valueChanged(int value);

  void on_move_z_slider_valueChanged(int value);

  void on_rotate_x_slider_valueChanged(int value);

  void on_rotate_y_slider_valueChanged(int value);

  void on_rotate_z_slider_valueChanged(int value);

  void on_choose_file_button_clicked();

  void on_zoom_slider_valueChanged(int value);

  void on_none_point_radio_clicked();

  void on_square_point_radio_clicked();

  void on_circle_point_radio_clicked();

  void on_point_size_spinBox_valueChanged(int arg1);

  void on_line_color_Button_clicked();

  void on_color_point_Button_clicked();

  void on_solid_line_radio_clicked();

  void on_line_size_spinBox_2_valueChanged(int arg1);

  void on_dashed_line_radio_clicked();

  void on_back_color_Button_clicked();

  void on_screen_Button_clicked();

  void on_gif_Button_clicked();

  void make_gif();

 private:
  Ui::MainWindow *ui;
  void free_memory();
  void reset_values();
  int image_count = 0;
  QGifImage *gif;
  QTimer timer;
  QImage image_gif[50];
};
#endif  // MAINWINDOW_H
