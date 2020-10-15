#ifndef FILTERS_H
#define FILTERS_H

#include <QImage>

//базовый класс для фильтров

class filter {
public:
    filter() {};
    ~filter() {};
	virtual QImage calculateNewImagePixMap(const QImage &photo, int radius) = 0;
};

//инвертирование

class invert : public filter {
public:
    invert() {};
    ~invert() {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius) override;
};

//матричный фильтр - базовый класс

class matrixFilter : public filter {
public:
	float* vector;
    int mRadius; //радиус матрицы
    matrixFilter(int radius = 1) : mRadius(radius) {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius) override;
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
};

//размытие

class blur : public matrixFilter {
public:
    blur() {
        int size = 2 * mRadius + 1; //размер матрицы, всегда нечетное число, чтобы был центр
        vector = new float[size * size];
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
                vector[i * size + j] = float(1.0f / ((float)(size * size)));
	}
};

class gaussianBlur : public matrixFilter {
public:
    gaussianBlur() {
		createGaussianVector(3, 2);
	}
    ~gaussianBlur() {};
	void createGaussianVector(int radius, int sigma);
};

class grayscale : public filter {
public:
    grayscale() {};
    ~grayscale() {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

class sepia : public filter {
public:
    sepia() {};
    ~sepia() {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

class brightness : public filter {
public:
    brightness() {};
    ~brightness() {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

class sobel : public matrixFilter {
public:
	float* vectorX;
	float* vectorY;
    sobel() {
		vectorX = new float[3 * 3];
		vectorX[0] = -1; vectorX[1] = 0; vectorX[2] = 1;
		vectorX[3] = -2; vectorX[4] = 0; vectorX[5] = 2;
		vectorX[6] = -1; vectorX[7] = 0; vectorX[8] = 1;
		vectorY = new float[3 * 3];
		vectorY[0] = -1; vectorY[1] = -2; vectorY[2] = -1;
        vectorY[3] =  0; vectorY[4] =  0; vectorY[5] =  0;
        vectorY[6] =  1; vectorY[7] =  2; vectorY[8] =  1;
	}
	QImage calculateNewImagePixMap(const QImage &photo, int radius) override;
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
};

class sharpness : public matrixFilter {
public:
	float* vector;
    sharpness() {
		vector = new float[3 * 3];
        vector[0] =  0; vector[1] = -1; vector[2] =  0;
        vector[3] = -1; vector[4] =  5; vector[5] = -1;
        vector[6] =  0; vector[7] = -1; vector[8] =  0;
	}
	QImage calculateNewImagePixMap(const QImage &photo, int radius) override;
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
};

class embossing : public matrixFilter {
public:
	float* vector;
    embossing() {
		vector = new float[3 * 3];
        vector[0] = 0; vector[1] =  1; vector[2] =  0;
        vector[3] = 1; vector[4] =  0; vector[5] = -1;
        vector[6] = 0; vector[7] = -1; vector[8] =  0;
	}
	QImage calculateNewImagePixMap(const QImage &photo, int radius) override;
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
};

class sharpnessMax : public matrixFilter {
public:
	float* vector;
    sharpnessMax() {
		vector = new float[3 * 3];
		vector[0] = -1; vector[1] = -1; vector[2] = -1;
        vector[3] = -1; vector[4] =  9; vector[5] = -1;
		vector[6] = -1; vector[7] = -1; vector[8] = -1;
	}
	QImage calculateNewImagePixMap(const QImage &photo, int radius) override;
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
};

class scharr : public matrixFilter {
public:
    float* vectorX;
    float* vectorY;
    scharr() {
        vectorX = new float[3 * 3];
        vectorX[0] =  3; vectorX[1] = 0; vectorX[2] =   -3;
        vectorX[3] = 10; vectorX[4] = 0; vectorX[5] =  -10;
        vectorX[6] =  3; vectorX[7] = 0; vectorX[8] =   -3;
        vectorY = new float[3 * 3];
        vectorY[0] =  3; vectorY[1] =  10; vectorY[2] =  3;
        vectorY[3] =  0; vectorY[4] =   0; vectorY[5] =  0;
        vectorY[6] = -3; vectorY[7] = -10; vectorY[8] = -3;
    }
    QImage calculateNewImagePixMap(const QImage &photo, int radius) override;
    QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
};

class moving : public filter {
public:
    moving() {};
    ~moving() {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

class glass : public filter {
public:
    glass() {};
    ~glass() {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

class dilation : public filter {
public:
    std::vector<int> vector;
    dilation() {
        vector.resize(9);
        vector[0] = 0; vector[1] = 1; vector[2] = 0;
        vector[3] = 1; vector[4] = 1; vector[5] = 1;
        vector[6] = 0; vector[7] = 1; vector[8] = 0;
	}
    void setVector(std::vector<int> element) {
        vector = element;
    }
    ~dilation() {};
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

class erosion : public filter {
public:
    std::vector<int> vector;
    erosion() {
        vector.resize(9);
        vector[0] = 0; vector[1] = 1; vector[2] = 0;
        vector[3] = 1; vector[4] = 1; vector[5] = 1;
        vector[6] = 0; vector[7] = 1; vector[8] = 0;
    }
    void setVector(std::vector<int> element) {
        vector = element;
    }
    ~erosion() {};
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

class opening : public filter {
public:
    std::vector<int> ovector;
    opening() {}
    ~opening() {};
    void setVector(std::vector<int> element) {
        ovector = element;
    }
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

class closing : public filter {
public:
    std::vector<int> cvector;
    closing() {}
    ~closing() {};
    void setVector(std::vector<int> element) {
        cvector = element;
    }
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

class idealReflector : public filter {
public:
    idealReflector() {};
    ~idealReflector() {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

class linearStretch : public filter {
public:
    linearStretch() {};
    ~linearStretch() {};
    QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

class blackHat : public filter {
public:
    std::vector<int> bhvector;
    blackHat() {
        bhvector.resize(9);
        bhvector[0] = 0; bhvector[1] = 1; bhvector[2] = 0;
        bhvector[3] = 1; bhvector[4] = 1; bhvector[5] = 1;
        bhvector[6] = 0; bhvector[7] = 1; bhvector[8] = 0;
    }
    void setVector(std::vector<int> element) {
        bhvector = element;
    }
    ~blackHat() {};
    QImage calculateNewImagePMap(const QImage &photo, int radius, std::vector<int> vec);
    QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

class sub : public filter {
public:
    std::vector<int> svector;
    sub() {
        svector.resize(9);
        svector[0] = 0; svector[1] = 1; svector[2] = 0;
        svector[3] = 1; svector[4] = 1; svector[5] = 1;
        svector[6] = 0; svector[7] = 1; svector[8] = 0;
    }
    void setVector(std::vector<int> element) {
        svector = element;
    }
    ~sub() {}
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

class medianFilter : public matrixFilter {
public:
    medianFilter() {}
    ~medianFilter() {}
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
    QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

#endif // FILTERS_H
