#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle(u8"Редактор фильтров");
    QPixmap pm(":/myImages/island.jpg");
    widthScaled = ui->image->width();
    heightScaled = ui->image->height();
    QImage temp(":/myImages/island.jpg");
    origin = temp;
    ui->image->setPixmap(pm.scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
    ui->image->setAlignment(Qt::AlignHCenter);
    elWindow = new elemWindow();
    // создаем связь между элементами меню и функциями для них
    connect(ui->structuring_element, SIGNAL(triggered()), this, SLOT(struct_element_slot()));
    connect(ui->open_file, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->save_file_as, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->exit, SIGNAL(triggered()), this, SLOT(exit()));
    connect(ui->origin, SIGNAL(triggered()), this, SLOT(origin_slot()));
    connect(ui->inversion, SIGNAL(triggered()), this, SLOT(invert_slot()));
    connect(ui->blur, SIGNAL(triggered()), this, SLOT(blur_slot()));
    connect(ui->gaussian_blur, SIGNAL(triggered()), this, SLOT(gaussian_blur_slot()));
    connect(ui->grayscale, SIGNAL(triggered()), this, SLOT(grayscale_slot()));
    connect(ui->sepia, SIGNAL(triggered()), this, SLOT(sepia_slot()));
    connect(ui->brightness, SIGNAL(triggered()), this, SLOT(brightness_slot()));
    connect(ui->sobel, SIGNAL(triggered()), this, SLOT(sobel_slot()));
    connect(ui->sharpness, SIGNAL(triggered()), this, SLOT(sharpness_slot()));
    connect(ui->sharpness_max, SIGNAL(triggered()), this, SLOT(sharpness_max_slot()));
    connect(ui->moving, SIGNAL(triggered()), this, SLOT(moving_slot()));
    connect(ui->scharr, SIGNAL(triggered()), this, SLOT(scharr_slot()));
    connect(ui->embossing, SIGNAL(triggered()), this, SLOT(embossing_slot()));
    connect(ui->brightness, SIGNAL(triggered()), this, SLOT(brightness_slot()));
    connect(ui->glass, SIGNAL(triggered()), this, SLOT(glass_slot()));
    connect(ui->ideal_reflector, SIGNAL(triggered()), this, SLOT(ideal_reflector_slot()));
    connect(ui->linear_stretch, SIGNAL(triggered()), this, SLOT(linear_stretch_slot()));
    connect(ui->dilation, SIGNAL(triggered()), this, SLOT(dilation_slot()));
    connect(ui->erosion, SIGNAL(triggered()), this, SLOT(erosion_slot()));
    connect(ui->opening, SIGNAL(triggered()), this, SLOT(opening_slot()));
    connect(ui->closing, SIGNAL(triggered()), this, SLOT(closing_slot()));
    connect(ui->black_hat, SIGNAL(triggered()), this, SLOT(black_hat_slot()));
    connect(ui->median_filter, SIGNAL(triggered()), this, SLOT(median_filter_slot()));
}

MainWindow::~MainWindow() {
    delete ui;
}
/*
 * слот для открытия файла
*/
void MainWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(NULL, QString::fromUtf8(u8"Выберите изображение"),
                                                    QString("."), QString("Image files (*.jpg *.jpeg *.png);;JPEG (*.jpg *.jpeg);;PNG Files (*.png)"));
    if (!fileName.isEmpty()) {
            QImage image(fileName);
            if (image.isNull()) {
                QMessageBox::information(this, tr("Error"),
                                         tr("Cannot load %1.").arg(fileName));
                return;
            }
            origin = image;
            QPixmap pixmap(QPixmap::fromImage(image));
            widthScaled = ui->image->width();
            heightScaled = ui->image->height();
            ui->image->setPixmap(pixmap.scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
            ui->image->setAlignment(Qt::AlignHCenter);

    }
}
/*
 * слот для сохранения файла
*/

void MainWindow::save() {
    QString fileName = QFileDialog::getSaveFileName(NULL, QString::fromUtf8(u8"Сохранить изображение"),
                                                    QString("."), QString("JPEG (*.jpg *.jpeg);;PNG Files (*.png)"));
    if (!fileName.isEmpty()) {
        last.save(fileName);
    }
}
/*
 * слот для выхода из программы
*/

void MainWindow::exit() {
    QMessageBox msgBox;
    msgBox.setText("Вы уверены?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int res = msgBox.exec();
    if (res == QMessageBox::Ok) {
            QApplication::quit();
    }
}

void MainWindow::origin_slot() {
    QPixmap pm(QPixmap::fromImage(origin));
    ui->image->setPixmap(pm.scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::invert_slot() {
    invert* image = new invert();
    QImage invertImage;
    invertImage = image->calculateNewImagePixMap(origin, 0);
    last = invertImage;
    ui->image->setPixmap(QPixmap::fromImage(invertImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::blur_slot() {
    blur* image = new blur();
    QImage blurImage;
    blurImage = image->calculateNewImagePixMap(origin, 1);
    last = blurImage;
    ui->image->setPixmap(QPixmap::fromImage(blurImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::gaussian_blur_slot() {
    gaussianBlur* image = new gaussianBlur();
    QImage gblurImage;
    gblurImage = image->calculateNewImagePixMap(origin, 3);
    last = gblurImage;
    ui->image->setPixmap(QPixmap::fromImage(gblurImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::grayscale_slot() {
    grayscale* image = new grayscale();
    QImage gsImage;
    gsImage = image->calculateNewImagePixMap(origin, 1);
    last = gsImage;
    ui->image->setPixmap(QPixmap::fromImage(gsImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::sepia_slot() {
    sepia* image = new sepia();
    QImage sepiaImage;
    sepiaImage = image->calculateNewImagePixMap(origin, 1);
    last = sepiaImage;
    ui->image->setPixmap(QPixmap::fromImage(sepiaImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::brightness_slot() {
    brightness* image = new brightness();
    QImage brightnessImage;
    brightnessImage = image->calculateNewImagePixMap(origin, 1);
    last = brightnessImage;
    ui->image->setPixmap(QPixmap::fromImage(brightnessImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::sobel_slot() {
    sobel* image = new sobel();
    QImage sobelImage;
    sobelImage = image->calculateNewImagePixMap(origin, 1);
    last = sobelImage;
    ui->image->setPixmap(QPixmap::fromImage(sobelImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::sharpness_slot() {
    sharpness* image = new sharpness();
    QImage sharpnessImage;
    sharpnessImage = image->calculateNewImagePixMap(origin, 1);
    last = sharpnessImage;
    ui->image->setPixmap(QPixmap::fromImage(sharpnessImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::embossing_slot() {
    embossing* image = new embossing();
    QImage embossingImage;
    embossingImage = image->calculateNewImagePixMap(origin, 1);
    last = embossingImage;
    ui->image->setPixmap(QPixmap::fromImage(embossingImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::sharpness_max_slot() {
    sharpnessMax* image = new sharpnessMax();
    QImage sharpnessMaxImage;
    sharpnessMaxImage = image->calculateNewImagePixMap(origin, 1);
    last = sharpnessMaxImage;
    ui->image->setPixmap(QPixmap::fromImage(sharpnessMaxImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::scharr_slot() {
    scharr* image = new scharr();
    QImage scharrImage;
    scharrImage = image->calculateNewImagePixMap(origin, 1);
    last = scharrImage;
    ui->image->setPixmap(QPixmap::fromImage(scharrImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::moving_slot() {
    moving* image = new moving();
    QImage moveImage;
    moveImage = image->calculateNewImagePixMap(origin, 1);
    last = moveImage;
    ui->image->setPixmap(QPixmap::fromImage(moveImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::glass_slot() {
    glass* image = new glass();
    QImage glassImage;
    glassImage = image->calculateNewImagePixMap(origin, 1);
    last = glassImage;
    ui->image->setPixmap(QPixmap::fromImage(glassImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::dilation_slot() {
    dilation* image = new dilation();
    image->setVector(structElement);
    QImage dilationImage;
    dilationImage = image->calculateNewImagePixMap(origin, 1);
    last = dilationImage;
    ui->image->setPixmap(QPixmap::fromImage(dilationImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::erosion_slot() {
    erosion* image = new erosion();
    image->setVector(structElement);
    QImage erosionImage;
    erosionImage = image->calculateNewImagePixMap(origin, 1);
    last = erosionImage;
    ui->image->setPixmap(QPixmap::fromImage(erosionImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::opening_slot() {
    opening* image = new opening();
    image->setVector(structElement);
    QImage openingImage;
    qDebug() << image->ovector;
    openingImage = image->calculateNewImagePixMap(origin, 1);
    last = openingImage;
    ui->image->setPixmap(QPixmap::fromImage(openingImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::closing_slot() {
    closing* image = new closing();
    image->setVector(structElement);
    QImage closingImage;
    qDebug() << image->cvector;
    closingImage = image->calculateNewImagePixMap(origin, 1);
    last = closingImage;
    ui->image->setPixmap(QPixmap::fromImage(closingImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::black_hat_slot() {
    blackHat* image = new blackHat();
    image->setVector(structElement);
    QImage bhImage;
    bhImage = image->calculateNewImagePixMap(origin, 1);
    last = bhImage;
    ui->image->setPixmap(QPixmap::fromImage(bhImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::ideal_reflector_slot() {
    idealReflector* image = new idealReflector();
    QImage irImage;
    irImage = image->calculateNewImagePixMap(origin, 1);
    last = irImage;
    ui->image->setPixmap(QPixmap::fromImage(irImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::linear_stretch_slot() {
    linearStretch* image = new linearStretch();
    QImage lsImage;
    lsImage = image->calculateNewImagePixMap(origin, 1);
    last = lsImage;
    ui->image->setPixmap(QPixmap::fromImage(lsImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::median_filter_slot() {
    medianFilter* image = new medianFilter();
    QImage mfImage;
    mfImage = image->calculateNewImagePixMap(origin, 1);
    last = mfImage;
    ui->image->setPixmap(QPixmap::fromImage(mfImage).scaled(widthScaled, heightScaled, Qt::KeepAspectRatio, Qt::FastTransformation));
}

void MainWindow::struct_element_slot() {
    elWindow->show();
}
