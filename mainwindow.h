#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

#include "filters.h"
#include "elemwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow {
    Q_OBJECT

private slots:
    void open();
    void save();
    void exit();
    void origin_slot();
    void invert_slot();
    void blur_slot();
    void gaussian_blur_slot();
    void grayscale_slot();
    void sepia_slot();
    void brightness_slot();
    void sobel_slot();
    void sharpness_slot();
    void embossing_slot();
    void scharr_slot();
    void moving_slot();
    void sharpness_max_slot();
    void glass_slot();
    void dilation_slot();
    void erosion_slot();
    void opening_slot();
    void closing_slot();
    void black_hat_slot();
    void ideal_reflector_slot();
    void linear_stretch_slot();
    void median_filter_slot();
    void struct_element_slot();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    elemWindow *elWindow;
    QLabel *image;
    QImage origin;
    QImage last;
    int widthScaled;
    int heightScaled;
};
#endif // MAINWINDOW_H
