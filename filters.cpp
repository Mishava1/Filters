#include "filters.h"

#include <cmath>
#include <algorithm>
#include <vector>
#include <QDebug>

/*
 * функция для крайних значений, если в результате вычислений получится,
 * к примеру, что одна из компонент пикселя равна 300 (чего быть не может),
 * то эта функция вместо этого значения вернет 255
*/
template <class T>
T clamp(T v, int max, int min) {
    if (v > max)
        return max;
    else if (v < min)
        return min;
    return v;
}

/*
 * инвертирование, каждая компонента цвета задается обратной себе
 * (черный цвет становится белым и т.д)
*/

QImage invert::calculateNewImagePixMap(const QImage &photo, int radius) {
    QImage result_Image(photo);
    for (int x = 0; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            QColor photoColor = photo.pixelColor(x, y);
            photoColor.setRgb(255 - photoColor.red(), 255 - photoColor.green(), 255 - photoColor.blue());
            result_Image.setPixelColor(x, y, photoColor);
        }
    return result_Image;
}
/*
 * матричный фильтр (функция для задания цвета отдельного пикселя)
 * задается матрица размера size, берется определенный пиксель и его соседи в радиусе size
 * происходит покомпонентное умножение полученной матрицы пикселей и матрицы фильтра,
 * в результате чего мы получаем ОДНО значение, которое мы подставляем на место исходного пикселя
 * (применив при этом функцию clamp, так как у некоторых пикселей нет соседей)
*/

QColor matrixFilter::calculateNewPixelColor(QImage photo, int x, int y, int radius) {
    float returnR = 0;
    float returnG = 0;
    float returnB = 0;
    float size = 2 * radius + 1;
    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++) {
            int idx = (i + radius)*size + j + radius;
            QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0),
                clamp<int>(y + i, photo.height() - 1, 0));
            returnR += color.red() * vector[idx];
            returnG += color.green() * vector[idx];
            returnB += color.blue() * vector[idx];
        }
    return QColor(clamp(returnR, 255, 0), clamp(returnG, 255, 0), clamp(returnB, 255, 0));
}

/*
 * предыдущая функция применятся последовательно к каждому пикселю изображения
*/

QImage matrixFilter::calculateNewImagePixMap(const QImage& photo, int radius) {
    QImage result_Image(photo);
    for (int x = 0; x < photo.width(); x++) {
        for (int y = 0; y < photo.height(); y++) {
            result_Image.setPixelColor(x, y, calculateNewPixelColor(photo, x, y, radius));
        }
    }
    return result_Image;
}

/*
 * сначала применяется обычное гауссово распределение, затем полученный результат
 * нормируется (поскольку сумма значений будет отличаться от одного, изображение без нормировки
 * будет либо темнеть, либо светлеть, что в данном случае недопустимо)
*/

void gaussianBlur::createGaussianVector(int radius, int sigma) {
    const unsigned int size = 2 * radius + 1;
    float norm = 0;
    vector = new float[size*size];
    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++) {
            int idx = (i + radius) * size + (j + radius);
            vector[idx] = exp(-(i * i + j * j) / (sigma*sigma));
            norm += vector[idx];
        }
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            vector[i * size + j] /= norm;
        }
}

/*
 * для каждого пикселя считается интенсивность (Intensity или Luminosity, определяет насколько ярким является пиксель),
 * которой можно заменить цвет (у всех серых цветов одинаковые компоненты,
 * у черного цвета (0, 0, 0), у белого (255, 255, 255))
*/

QImage grayscale::calculateNewImagePixMap(const QImage &photo, int radius) {
    QImage result_Image(photo);
    for (int x = 0; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            QColor photoColor = photo.pixelColor(x, y);
            int intensity = int(0.299 * photoColor.red() + 0.587 *photoColor.green() + 0.114 * photoColor.blue());
            photoColor.setRgb(intensity, intensity, intensity);
            result_Image.setPixelColor(x, y, photoColor);
        }
    return result_Image;
}

/*
 * сначала мы переводим изображение в серые оттенки, потом при помощи коэффициентов
 * мы делаем цвета более желтыми, песочными (вычитание у синего происходит из-за того, что
 * желтый цвет в модели rgb задается при помощи красного и зеленого)
*/

QImage sepia::calculateNewImagePixMap(const QImage &photo, int radius) {
    QImage result_Image(photo);
    for (int x = 0; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            int k = 13;
            QColor photoColor = photo.pixelColor(x, y);
            int intensity = int(0.299 * photoColor.red() + 0.587 *photoColor.green() + 0.114 * photoColor.blue());
            photoColor.setRgb(clamp<int>(intensity + 2 * k, 255, 0), clamp<int>(intensity + 0.5 * k, 255, 0), clamp<int>(intensity - 1 * k, 255, 0));
            result_Image.setPixelColor(x, y, photoColor);
        }
    return result_Image;
}

/*
 * повышаем яркость изображения за счет прибавления константы к каждой компоненте цвета
*/

QImage brightness::calculateNewImagePixMap(const QImage &photo, int radius) {
    QImage result_Image(photo);
    for (int x = 0; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            int k = 25;
            QColor photoColor = photo.pixelColor(x, y);
            photoColor.setRgb(clamp<int>(photoColor.red() + k, 255, 0), clamp<int>(photoColor.green(), 255, 0), clamp<int>(photoColor.blue(), 255, 0));
            result_Image.setPixelColor(x, y, photoColor);
        }
    return result_Image;
}

/*
 * обычный матричный фильтр, но перемножение происходит с двумя матрицами
*/

QColor sobel::calculateNewPixelColor(QImage photo, int x, int y, int radius) {
    float returnR = 0;
    float returnG = 0;
    float returnB = 0;
    float size = 2 * radius + 1;
    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++) {
            int idx = (i + radius)*size + j + radius;
            QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0),
                clamp<int>(y + i, photo.height() - 1, 0));
            returnR += color.red() * vectorX[idx] + color.red() * vectorY[idx];
            returnG += color.green() * vectorX[idx] + color.green() * vectorY[idx];
            returnB += color.blue() * vectorX[idx] + color.blue() * vectorY[idx];
        }
    return QColor(clamp(returnR, 255, 0), clamp(returnG, 255, 0), clamp(returnB, 255, 0));
}


QImage sobel::calculateNewImagePixMap(const QImage& photo, int radius) {
    QImage result_Image(photo);
    for (int x = 0; x < photo.width(); x++) {
        for (int y = 0; y < photo.height(); y++) {
            result_Image.setPixelColor(x, y, calculateNewPixelColor(photo, x, y, radius));
        }
    }
    return result_Image;
}

QColor sharpness::calculateNewPixelColor(QImage photo, int x, int y, int radius) {
    float returnR = 0;
    float returnG = 0;
    float returnB = 0;
    float size = 2 * radius + 1;
    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++) {
            int idx = (i + radius) * size + j + radius;
            QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0), clamp<int>(y + i, photo.height() - 1, 0));
            returnR += color.red() * vector[idx];
            returnG += color.green() * vector[idx];
            returnB += color.blue() * vector[idx];
        }
    return QColor(clamp(returnR, 255, 0), clamp(returnG, 255, 0), clamp(returnB, 255, 0));
}
QImage sharpness::calculateNewImagePixMap(const QImage& photo, int radius) {
    QImage result_Image(photo);
    for (int x = 0; x < photo.width(); x++) {
        for (int y = 0; y < photo.height(); y++) {
            result_Image.setPixelColor(x, y, calculateNewPixelColor(photo, x, y, radius));
        }
    }
    return result_Image;
}

/*
 * тиснение: перевели в ч/б, умножили на матрицу, прибавили константу
*/

QColor embossing::calculateNewPixelColor(QImage photo, int x, int y, int radius) {
    float returnR = 0;
    float returnG = 0;
    float returnB = 0;
    float size = 2 * radius + 1;
    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++) {
            int idx = (i + radius) * size + j + radius;
            QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0), clamp<int>(y + i, photo.height() - 1, 0));
            returnR += vector[idx] * (0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue()) + 15;
            returnG += vector[idx] * (0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue()) + 15;
            returnB += vector[idx] * (0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue()) + 15;
        }
    return QColor(clamp(returnR, 255, 0), clamp(returnG, 255, 0), clamp(returnB, 255, 0));
}


QImage embossing::calculateNewImagePixMap(const QImage& photo, int radius) {
    QImage result_Image(photo);
    for (int x = 0; x < photo.width(); x++) {
        for (int y = 0; y < photo.height(); y++) {
            result_Image.setPixelColor(x, y, calculateNewPixelColor(photo, x, y, radius));
        }
    }
    return result_Image;
}

QColor sharpnessMax::calculateNewPixelColor(QImage photo, int x, int y, int radius) {
    float returnR = 0;
    float returnG = 0;
    float returnB = 0;
    float size = 2 * radius + 1;
    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++) {
            int idx = (i + radius) * size + j + radius;
            QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0),
                clamp<int>(y + i, photo.height() - 1, 0));
            returnR += color.red() * vector[idx];
            returnG += color.green() * vector[idx];
            returnB += color.blue() * vector[idx];
        }
    return QColor(clamp(returnR, 255, 0), clamp(returnG, 255, 0), clamp(returnB, 255, 0));
}

QImage sharpnessMax::calculateNewImagePixMap(const QImage& photo, int radius) {
    QImage result_Image(photo);
    for (int x = 0; x < photo.width(); x++) {
        for (int y = 0; y < photo.height(); y++) {
            result_Image.setPixelColor(x, y, calculateNewPixelColor(photo, x, y, radius));
        }
    }
    return result_Image;
}

/*
 * аналогично фильтру собеля
*/
QColor scharr::calculateNewPixelColor(QImage photo, int x, int y, int radius) {
    float returnR = 0;
    float returnG = 0;
    float returnB = 0;
    float size = 2 * radius + 1;
    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++) {
            int idx = (i + radius)*size + j + radius;
            QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0),
                clamp<int>(y + i, photo.height() - 1, 0));
            returnR += color.red() * vectorX[idx] + color.red() * vectorY[idx];
            returnG += color.green() * vectorX[idx] + color.green() * vectorY[idx];
            returnB += color.blue() * vectorX[idx] + color.blue() * vectorY[idx];
        }
    return QColor(clamp(returnR, 255, 0), clamp(returnG, 255, 0), clamp(returnB, 255, 0));
}

QImage scharr::calculateNewImagePixMap(const QImage& photo, int radius) {
    QImage result_Image(photo);
    for (int x = 0; x < photo.width(); x++) {
        for (int y = 0; y < photo.height(); y++) {
            result_Image.setPixelColor(x, y, calculateNewPixelColor(photo, x, y, radius));
        }
    }
    return result_Image;
}

/*
 * перенесли изображение влево на 50 пикселей, правую часть залили черным цветом
*/
QImage moving::calculateNewImagePixMap(const QImage& photo, int radius) {
    QImage result_Image(photo);
    for (int x = 0; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            int k = x + 50, l = y;
            if ((k >= photo.width()) || (k < 0)) k = x;
            QColor newColor = photo.pixelColor(k, l);
            result_Image.setPixelColor(x, y, newColor);
        }
    for (int x = photo.width() - 50; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            QColor newColor;
            newColor.setRgb(0,0,0);
            result_Image.setPixelColor(x, y, newColor);
        }
    return result_Image;
}

/*
 * стекло: присвоили цвет пикселю из случайного пикселя в некоторой окрестности первоначалаьного пикселя
*/

QImage glass::calculateNewImagePixMap(const QImage &photo, int radius) {
    QImage result_Image(photo);
    srand(time(0));
    for (int x = 0; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            int k = x + ((double)(rand()) / RAND_MAX - 0.5) * 10,
                l = y + ((double)(rand()) / RAND_MAX - 0.5) * 10;
            if (k >= photo.width() || k < 0) { k = x; }
            if (l >= photo.height() || l < 0) { l = y; }
            QColor newColor = photo.pixelColor(k, l);
            result_Image.setPixelColor(x, y, newColor);
        }
    return result_Image;
}

/*
 * фильтры мат. морфологии (тоже являются матричными)
 * расширение
 * изначальный цвет возвращаемого пикселя - черный
 * берем интенсивность пикселя из окрестности и смотрим,
 * является ли соответствующий ему элемент матрицы положительным числом
 * и максимальная ли у него интенсивность в этой окрестности
 * если да - то ставим его вместо возвращаемого пикселя
 * в результате выполнения светлые участки увеличатся
*/

QColor dilation::calculateNewPixelColor(QImage photo, int x, int y, int radius) {
    float returnR = 0;
    float returnG = 0;
    float returnB = 0;
    int kernel = (sqrt(vector.size())) / 2;
    int max = 0;
    for (int i = -kernel; i <= kernel; i++)
        for (int j = -kernel; j <= kernel; j++) {
            QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0), clamp<int>(y + i, photo.height() - 1, 0));
            int intensity = color.red();
            if (color.red() != color.green() || color.red() != color.blue() || color.green() != color.blue()) {
                intensity = (int)(0.299*color.red() + 0.587*color.green() + 0.114*color.blue());
            }
            if ((vector[i + j +  sqrt(vector.size()) * 2] > 0) && (intensity > max)) {
                max = intensity;
                returnR = color.red();
                returnG = color.green();
                returnB = color.blue();
            }
        }
    return QColor(clamp(returnR, 255, 0), clamp(returnG, 255, 0), clamp(returnB, 255, 0));

}

QImage dilation::calculateNewImagePixMap(const QImage &photo, int radius) {
    QImage result_Image(photo);
    for (int x = 0; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            result_Image.setPixelColor(x, y, calculateNewPixelColor(photo, x, y, radius));
        }
    return result_Image;
}

/*
 * сужение
 * изначальный цвет возвращаемого пикселя - белый
 * берем интенсивность пикселя из окрестности и смотрим,
 * является ли соответствующий ему элемент матрицы положительным числом
 * и минимальная ли у него интенсивность в этой окрестности
 * если да - то ставим его вместо возвращаемого пикселя
 * в результате выполнения темные участки увеличатся
*/

QColor erosion::calculateNewPixelColor(QImage photo, int x, int y, int radius) {
    float returnR = 255;
    float returnG = 255;
    float returnB = 255;
    int kernel =  sqrt(vector.size()) / 2;
    int min = 255;
    for (int i = -kernel; i <= kernel; i++)
        for (int j = -kernel; j <= kernel; j++) {
            QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0), clamp<int>(y + i, photo.height() - 1, 0));
            int intensity = color.red();
            if (color.red() != color.green() || color.red() != color.blue() || color.green() != color.blue()) {
                intensity = (int)(0.299*color.red() + 0.587*color.green() + 0.114*color.blue());
            }
            if ((vector[i + j +  sqrt(vector.size()) * 2] > 0) && (intensity < min)) {
                min = intensity;
                returnR = color.red();
                returnG = color.green();
                returnB = color.blue();
            }
        }
    return QColor(clamp(returnR, 255, 0), clamp(returnG, 255, 0), clamp(returnB, 255, 0));

}

QImage erosion::calculateNewImagePixMap(const QImage &photo, int radius) {
    QImage result_Image(photo);
    for (int x = 0; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            result_Image.setPixelColor(x, y, calculateNewPixelColor(photo, x, y, radius));
        }
    return result_Image;

}

/*
 * открытие = сужение + расширение (используется, если на фоне нет шума)
*/
QImage opening::calculateNewImagePixMap(const QImage &photo, int radius) {
    QImage result_Image(photo);
    erosion* er;
    er = new erosion();
    er->setVector(ovector);
    qDebug() << er->vector;
    result_Image = er->calculateNewImagePixMap(result_Image, 1);
    dilation* dil;
    dil = new dilation();
    dil->setVector(ovector);
    qDebug() << dil->vector;
    qDebug() << ovector;
    result_Image = dil->calculateNewImagePixMap(result_Image, 1);
    return result_Image;
}

/*
 * закрытие = расширение + сужение (используется, если на фоне нет шума)
*/
QImage closing::calculateNewImagePixMap(const QImage &photo, int radius) {
    QImage result_Image(photo);
    erosion* er;
    dilation* dil;
    dil = new dilation();
    dil->setVector(cvector);
    qDebug() << dil->vector;
    result_Image = dil->calculateNewImagePixMap(result_Image, 1);
    er = new erosion();
    er->setVector(cvector);
    qDebug() << er->vector;
    result_Image = er->calculateNewImagePixMap(result_Image, 1);
    return result_Image;
}


/*
 * идеальный отражатель: находим максимумы в каждом из каналов,
 * затем масштабируем яркости пикселей
*/
QImage idealReflector::calculateNewImagePixMap(const QImage &photo, int radius) {
    QImage result_Image(photo);
    int r = 0, g = 0, b = 0;
    for (int x = 0; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            QColor color = photo.pixelColor(x, y);
            if (r < color.red()) {
                r = color.red();
            }
            if (g < color.green()) {
                g = color.green();
            }
            if (b < color.blue()) {
                b = color.blue();
            }
        }
    for (int x = 0; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            QColor color = photo.pixelColor(x, y);
            int r0 = color.red(), g0 = color.green(), b0 = color.blue();
            int newr = r0 * 255 / (r - 20), newg = g0 * 255 / (g - 20), newb = b0 * 255 / (b - 20);
            color.setRgb(clamp<int>(newr, 255, 0), clamp<int>(newg, 255, 0), clamp<int>(newb, 255, 0));
            result_Image.setPixelColor(x, y, color);
        }
    return result_Image;
}

/*
 * линейное растяжение гистограммы: находим маскимальные и минимальные интенсивности,
 * потом для каждого пикселя вычисляем подходящую ему интенсивность и устанавливаем ее
*/
QImage linearStretch::calculateNewImagePixMap(const QImage &photo, int radius) {
    QImage result_Image(photo);
    int inten, intMax = 0, intMin = 255;
    for (int x = 0; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            QColor color = photo.pixelColor(x, y);
            inten = color.value();
            if (inten > intMax) {
                intMax = inten;
            }
            if (inten < intMin) {
                intMin = inten;
            }
        }
    if (intMin == intMax) {
        intMax++;
    }
    qDebug() << intMax;
    qDebug() << intMin;
    for (int x = 0; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            QColor color = photo.pixelColor(x, y);
            int intRes;
            inten = color.value();
            intRes = (inten - intMin) * 255 / (intMax - intMin);
            color.setHsv(color.hsvHue(), color.hsvSaturation(), clamp(intRes, 255, 0));
            result_Image.setPixelColor(x, y, color);
        }
    return result_Image;
}

/*
 * мат морфология: black hat
 * применяем к изображению закрытие,
 * затем из каждого полученного канала вычитаем канал исходного
*/
QImage blackHat::calculateNewImagePixMap(const QImage &photo, int radius) {
    QImage result_Image(photo);
    QImage src_Image(photo);
    sub* s;
    s = new sub();
    s->setVector(bhvector);
    result_Image = s->calculateNewImagePixMap(result_Image, 1);
    return result_Image;
}

QImage sub::calculateNewImagePixMap(const QImage &photo, int radius) {
    QImage res(photo);
    QImage src(photo);
    closing* cl;
    cl = new closing();
    cl->setVector(svector);
    res = cl->calculateNewImagePixMap(src, 1);
    for (int x = 0; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            int resR = clamp(res.pixelColor(x,y).red() - src.pixelColor(x,y).red() , 255, 0);
            int resG = clamp(res.pixelColor(x,y).green() - src.pixelColor(x,y).green() , 255, 0);
            int resB = clamp(res.pixelColor(x,y).blue() - src.pixelColor(x,y).blue(), 255, 0);
            QColor resCol;
            resCol.setRgb(resR, resG, resB);
            res.setPixelColor(x, y, resCol);
        }
    return res;
}

/*
 * медианный фильтр: создаем вектор (массив) интенсивностей, сортируем его и находим медиану
 * после чего возвращаем эту медианную интенсивность
*/

QColor medianFilter::calculateNewPixelColor(QImage photo, int x, int y, int radius) {
    float size = 2 * radius + 1;
    std::vector<float> intVector(size * size, 0);
    std::vector<int> sortIntVector(size * size, 0);
    int median;
    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            int idx = (i + radius) * size + (j + radius);
            QColor color = photo.pixelColor(clamp(x + j, photo.width() - 1, 0), clamp(y + i, photo.height() - 1, 0));
            intVector[idx] = (0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue());
            sortIntVector[idx] = intVector[idx];
        }
    }
    sort(sortIntVector.begin(), sortIntVector.end());
    median = sortIntVector[size * size / 2];
    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++) {
            QColor color = photo.pixelColor(clamp(x + j, photo.width() - 1, 0), clamp(y + i, photo.height() - 1, 0));
            int inten = (int)(0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue());
            if (inten == median) {
                return photo.pixelColor(clamp(x + j, photo.width() - 1, 0),
                    clamp(y + i, photo.height() - 1, 0));
            }
        }
    return photo.pixelColor(x, y);
}

QImage medianFilter::calculateNewImagePixMap(const QImage &photo, int radius) {
    QImage result_Image(photo);
    for (int x = 0; x < photo.width(); x++)
        for (int y = 0; y < photo.height(); y++) {
            result_Image.setPixelColor(x, y, calculateNewPixelColor(photo, x, y, radius));
        }
    return result_Image;

}
