#include "glwidget.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QtMath>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QSlider>
#include "mainwindow.h"

//! [0]
GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    qDebug() << __FUNCTION__;
    setAutoFillBackground(false);

    // Alex
    _currentCenterInWorld = QPoint(-1, -1);
    _currentCenterInBase = QPoint(-1, -1);

    _minZoom = 1;
    _maxZoom = 7;
    _currentZoom = -1;

    _rasterWidth = -1;
    _rasterHeight = -1;

    _baseWidth = -1;
    _baseHeight = -1;
    _cachePath = QString("");

    _widgetWidth = 256;
    _widgetHeight = 256;

    GDALAllRegister();

    _mousePressed = false;
    _mousePressedCoords = QPointF(-1, -1);

    _cachePath = QString::null;
    _rasterDataPath = QString::null;

    _showClouds = false;

    _cropRoi = false;

    _subRoiSize = 16;
}

void GLWidget::wheelEvent(QWheelEvent * event )
{
    if (_currentZoom == -1)
    {
        return;
    }

    float shiftX = event->pos().x() - _widgetWidth/2;
    float shiftY = event->pos().y() - _widgetHeight/2;

    if(event->delta() > 1)
    {
        if (_currentZoom < _maxZoom)
        {
            _currentZoom++;

//            shiftX *= 2;
//            shiftY *= 2;
            _currentCenterInBase.setX(_currentCenterInBase.x() * 2 + shiftX);
            _currentCenterInBase.setY(_currentCenterInBase.y() * 2 + shiftY);

            _baseWidth *= 2;
            _baseHeight *= 2;
        }
    }
    else if (event->delta() < 1)
    {
        if (_currentZoom - 1 == 1)
        {
            _currentZoom--;

            _baseWidth = qPow(2, 8 + _currentZoom);
            _baseHeight = qPow(2, 8 + _currentZoom);

            _currentCenterInWorld = QPointF(_rasterWidth / 2, _rasterHeight / 2);
            _currentCenterInBase = QPointF(_baseWidth / 2, _baseHeight / 2);
        }
        else if (_currentZoom > _minZoom)
        {
            _currentZoom--;

//            shiftX /= 2;
//            shiftY /= 2;
            _currentCenterInBase.setX(_currentCenterInBase.x() / 2 - shiftX);
            _currentCenterInBase.setY(_currentCenterInBase.y() / 2 - shiftY);

            _baseWidth /= 2;
            _baseHeight /= 2;
        }
    }

    this->update();
    emit SET_ZOOM(_currentZoom);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (_mousePressed)
    {
        float shiftX = event->pos().x() - _mousePressedCoords.x();
        float shiftY = event->pos().y() - _mousePressedCoords.y();

        if (sqrt(qPow(shiftX, 2) + qPow(shiftY, 2)) > 50)
        {
            float tileWidthInWorld = _rasterWidth / qPow(2, _currentZoom);
            float tileHeightInWorld = _rasterHeight/ qPow(2, _currentZoom);
            QPointF newCurrentCenterInBase(_currentCenterInBase.x() - shiftX, _currentCenterInBase.y() - shiftY);
            QPointF newCurrentCenterInWorld(_currentCenterInWorld.x() - (shiftX / qPow(2, 8) * tileWidthInWorld),
                                            _currentCenterInWorld.y() - (shiftY / qPow(2, 8) * tileHeightInWorld));

            if (newCurrentCenterInBase.x() - qPow(2, 8) > 0 && newCurrentCenterInBase.x() + qPow(2, 8) < _baseWidth &&
                    newCurrentCenterInBase.y() - qPow(2, 8) > 0 && newCurrentCenterInBase.y() + qPow(2, 8) < _baseHeight)
            {
                _currentCenterInBase = newCurrentCenterInBase;
                _currentCenterInWorld = newCurrentCenterInWorld;

                _mousePressedCoords.setX(_mousePressedCoords.x() + shiftX);
                _mousePressedCoords.setY(_mousePressedCoords.y() + shiftY);

                update();
            }
        }
    }
}
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    _mousePressed = true;
    _mousePressedCoords.setX(event->pos().x());
    _mousePressedCoords.setY(event->pos().y());

    QApplication::setOverrideCursor(Qt::ClosedHandCursor);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    _mousePressed = false;
    _mousePressedCoords.setX(-1);
    _mousePressedCoords.setY(-1);

    QApplication::restoreOverrideCursor();
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, _widgetWidth, _widgetHeight);
    painter.end();

    if (_currentZoom == -1)
    {
        return;
    }

    QPointF frameTopLeftCornerInBase(_currentCenterInBase.x() - _widgetWidth/2, _currentCenterInBase.y() - _widgetHeight/2); // координа левого верхнего угла рамки в СК подложки

    QPointF realFrameTopLeftCornerInBase(qFloor(frameTopLeftCornerInBase.x() / qPow(2, 8)) * qPow(2, 8),
                                         qFloor(frameTopLeftCornerInBase.y() / qPow(2, 8)) * qPow(2, 8));

    float yReal = realFrameTopLeftCornerInBase.y();
    QImage allTilesQImage(_widgetWidth, _widgetHeight, QImage::Format_RGB888);
    QPainter allTilesPainter;
    allTilesPainter.begin(&allTilesQImage);

    while (yReal < (frameTopLeftCornerInBase.y() + _widgetHeight))
    {
        float xReal = realFrameTopLeftCornerInBase.x();
        while (xReal < (frameTopLeftCornerInBase.x() + _widgetWidth))
        {
            painter.begin(this);

            QString realTileName = xyz2qrst(QPoint(qRound(xReal / qPow(2, 8)), qRound(yReal / qPow(2, 8))), _currentZoom);
            QString realTilePath = _cachePath + "/" + QString::number(_currentZoom) + "/" + realTileName + ".png";
            QImage realTileImg(realTilePath);

            if (_widgetWidth - _baseWidth > 0 || _widgetHeight - _baseHeight > 0)
            {
                if (xReal - frameTopLeftCornerInBase.x() < (_widgetWidth - _baseWidth) / 2 || xReal - frameTopLeftCornerInBase.x() > _baseWidth ||
                        yReal - frameTopLeftCornerInBase.y() < (_widgetHeight - _baseHeight) / 2 || yReal - frameTopLeftCornerInBase.y() > _baseHeight)
                {
                    QImage blackTile(256, 256, QImage::Format_RGB888);
                    blackTile.fill(Qt::black);
                    allTilesPainter.drawImage(xReal - frameTopLeftCornerInBase.x(), yReal - frameTopLeftCornerInBase.y(), blackTile);
                }
                else
                {
                    allTilesPainter.drawImage(xReal - frameTopLeftCornerInBase.x(), yReal - frameTopLeftCornerInBase.y(), realTileImg);
                }
            }
            else
            {
                allTilesPainter.drawImage(xReal - frameTopLeftCornerInBase.x(), yReal - frameTopLeftCornerInBase.y(), realTileImg);
            }

            painter.end();

            xReal += qPow(2, 8);
        }

        yReal += qPow(2, 8);
    }

    allTilesPainter.end();

    painter.begin(this);

    QImage allTilesQImageGray = allTilesQImage.convertToFormat(QImage::Format_Grayscale8);
    uchar* data = allTilesQImageGray.bits();
    Mat allTilesCvImage(_widgetWidth, _widgetHeight, CV_8UC1, data);

    // Gamma correction
    Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    double gamma_ = 0.4;
    for( int i = 0; i < 256; ++i)
        p[i] = saturate_cast<uchar>(qPow(i / 255.0, gamma_) * 255.0);

    Mat allTilesCvImageEqualized = allTilesCvImage.clone();
    LUT(allTilesCvImage, lookUpTable, allTilesCvImageEqualized);

    imshow("GAMMA CORRECTED IMAGE", allTilesCvImageEqualized);

    // Detect clouds
    if (_showClouds) detectCloud(allTilesCvImageEqualized, 0.4, Size(_subRoiSize, _subRoiSize), allTilesQImage);

    painter.drawImage(0, 0, allTilesQImage);
    painter.end();

    // Crop roi
    if (_cropRoi)
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
            tr("*.bmp"));
        if (fileName != QString::null && !fileName.isEmpty())
        {
            allTilesQImageGray.save(fileName);
        }
        _cropRoi = false;
    }
}

void GLWidget::INITIALIZE_OPENGL_VIEWER(int rasterWidth, int rasterHeight, QString rasterDataPath, QString rasterCachePath, int viewBoxSize, int subRoiSize)
{
    _currentZoom = 1;

    _rasterWidth = rasterWidth;
    _rasterHeight = rasterHeight;

    _baseWidth = qPow(2, 8 + _currentZoom);
    _baseHeight = qPow(2, 8 + _currentZoom);

    _currentCenterInWorld = QPointF(_rasterWidth / 2, _rasterHeight / 2);
    _currentCenterInBase = QPointF(_baseWidth / 2, _baseHeight / 2);

    _widgetWidth = viewBoxSize;
    _widgetHeight = viewBoxSize;

    _subRoiSize = subRoiSize;

    setMinimumSize(QSize(_widgetWidth, _widgetHeight));
    setMaximumSize(QSize(_widgetWidth, _widgetHeight));

    if (!QDir(_cachePath).exists())
    {
        QMessageBox::information(this, "Ошибка", "Папка \"cache/\" с кэшом изображения не найдена!\n(*) \"cache/\" должна располагаться рядом с изображением");
        return;
    }

    _cachePath = rasterCachePath;
    _rasterDataPath = rasterDataPath;

    int layer = 1;
    QString layerFolder = _cachePath + "/";
    while (QDir(layerFolder + QString::number(layer)).exists())
    {
        layer++;
    }
    _maxZoom = --layer;
    emit SET_ZOOM(_currentZoom);
    update();
}

QString GLWidget::xyz2qrst(const QPoint &coords, int zoom)  // 3/2/5 ИЛИ 2/3/1 ~~~ zoom/x/y
{
    int qX = coords.x(); // частное
    int rX = 0; // остаток

    int qY = coords.y(); // частное
    int rY = 0; // остаток

    QString xCoordBin;
    QString yCoordBin;

    QStack<int> binCoord_X;
    QStack<int> binCoord_Y;

    // get binary X,Y
    for (int i = 0; i < zoom; i++)
    {
        rX = qX % 2;
        qX = qX / 2;

        binCoord_X.push(rX);

        rY = qY % 2;
        qY = qY / 2;

        binCoord_Y.push(rY);
    }

    while (!binCoord_X.isEmpty()) xCoordBin += QString::number(binCoord_X.pop());

    while (!binCoord_Y.isEmpty()) yCoordBin += QString::number(binCoord_Y.pop());

    QString qrst_dec; // 0, 1, 2, 3
    for (int i = 0; i < xCoordBin.size(); i++)
    {
        qrst_dec += QString::number(xCoordBin[i].digitValue() + 2 * yCoordBin[i].digitValue());
    }

    QString qrst_string;
    for (int i = 0; i < qrst_dec.size(); i++)
    {
        switch (qrst_dec[i].digitValue())
        {
        case 0:
            qrst_string += "q";
            break;
        case 1:
            qrst_string += "r";
            break;
        case 2:
            qrst_string += "s";
            break;
        case 3:
            qrst_string += "t";
            break;
        }
    }

    return qrst_string;
}

bool GLWidget::detectCloud(const Mat &curRoi, float c, const cv::Size &subRoiSize, QImage &qCurRoi)
{
    // АЛГОРИТМ 3-х ПРОХОДНЫЙ
    // НУЖНО ЧТО-ТО С ЭТИМ ДЕЛАТЬ

    bool cloudsHaveBeenDetected = false;

    QPainter painter(&qCurRoi);
    painter.setOpacity(0.25);

    Mat kernel(3, 3, CV_32FC1);
    kernel.at<float>(0) = -1;
    kernel.at<float>(1) = -1;
    kernel.at<float>(2) = -1;

    kernel.at<float>(3) = -1;
    kernel.at<float>(4) = 8;
    kernel.at<float>(5) = -1;

    kernel.at<float>(6) = -1;
    kernel.at<float>(7) = -1;
    kernel.at<float>(8) = -1;

    // Apply filter
    Mat laplaceRoi;
    filter2D(curRoi, laplaceRoi, CV_32FC1, kernel, Point(-1,-1)); // result в тонах серого

    int subRoiWidth = subRoiSize.width;
    int subRoiHeight = subRoiSize.height;

    int nSubRoiAlongX = curRoi.cols / subRoiWidth;
    int nSubRoiAlongY = curRoi.rows / subRoiHeight;

    Mat subRoiBrightnesses(nSubRoiAlongY, nSubRoiAlongX, CV_8UC1);
    Mat subRoiLaplaceValues(nSubRoiAlongY, nSubRoiAlongX, CV_32FC1);

    for (int r = 0; r < laplaceRoi.rows; r++)
    {
        for (int c = 0; c < laplaceRoi.cols; c++)
        {
            if (laplaceRoi.at<float>(r, c) < 0) laplaceRoi.at<float>(r, c) = 0;
        }
    }

    Mat laplaceRoiNormalized;
    convertScaleAbs(laplaceRoi, laplaceRoiNormalized, 1, 0);
    cv::normalize(laplaceRoi, laplaceRoiNormalized, 255, 0, NORM_INF);

    Mat laplaceImageForShowing;
    laplaceRoiNormalized.convertTo(laplaceImageForShowing, CV_8UC1);
    imshow("LAPLACE", laplaceImageForShowing);

    int nSteps = 0;
    int minLaplaceValue;
    int maxLaplaceValue;
    // Calculate min background's brightness
    // Calculate max background's brightness
    // Calculate min laplace value
    // Calculate max laplace value
    for (int r = 0; r < nSubRoiAlongY; r++)
    {
        for (int c = 0; c < nSubRoiAlongX; c++)
        {
            Mat laplaceSubRoi = Mat(laplaceRoi, Rect(0 + c * subRoiWidth, 0 + r * subRoiHeight, subRoiWidth, subRoiHeight));
            Scalar subRoiLaplaceValue = cv::mean(laplaceSubRoi);
            subRoiLaplaceValues.at<float>(r, c) = subRoiLaplaceValue.val[0];

            Mat sourceSubRoi = Mat(curRoi, Rect(0 + c * subRoiWidth, 0 + r * subRoiHeight, subRoiWidth, subRoiHeight));
            Scalar subRoiBrightness = cv::mean(sourceSubRoi);
            subRoiBrightnesses.at<uchar>(r, c) = subRoiBrightness.val[0];

            if (nSteps > 0)
            {
                if (subRoiLaplaceValue.val[0] < minLaplaceValue) minLaplaceValue = subRoiLaplaceValue.val[0];
                if (subRoiLaplaceValue.val[0] > maxLaplaceValue) maxLaplaceValue = subRoiLaplaceValue.val[0];
            }
            else
            {
                minLaplaceValue = subRoiLaplaceValue.val[0];
                maxLaplaceValue = subRoiLaplaceValue.val[0];
            }

            nSteps++;
        }
    }

    float laplace_threshold = minLaplaceValue + c * (maxLaplaceValue - minLaplaceValue);
    laplace_threshold = qMax(laplace_threshold, (float) 5);

    QVector<int> backgroundBrightnesses;
    // Perform background brightnesses
    for (int r = 0; r < nSubRoiAlongY; r++)
    {
        for (int c = 0; c < nSubRoiAlongX; c++)
        {
            if (subRoiLaplaceValues.at<float>(r, c) > laplace_threshold) backgroundBrightnesses.push_back(subRoiBrightnesses.at<uchar>(r, c));
        }
    }

    // Sort background brightnesses (vector<uchar>)
    if (!backgroundBrightnesses.empty())
    {
        for (int i = 0; i < backgroundBrightnesses.size(); i++)
        {
            int minBrightnessIndex = i;
            for (int j = i; j < backgroundBrightnesses.size(); j++)
            {
                if (backgroundBrightnesses[j] < backgroundBrightnesses[minBrightnessIndex])
                {
                    minBrightnessIndex = j;
                }
                qSwap(backgroundBrightnesses[i], backgroundBrightnesses[minBrightnessIndex]);
            }
        }
    }
    else
    {
        cloudsHaveBeenDetected = true;
        return cloudsHaveBeenDetected;
    }

    int maxBackgroundBrightness = backgroundBrightnesses[backgroundBrightnesses.size() - 1];
    int minBackgroundBrightness = backgroundBrightnesses[0];

    // Find median of background brightnesses
    qDebug() << backgroundBrightnesses;
    int medianBackgroundBrightness = backgroundBrightnesses[backgroundBrightnesses.size() / 2];
    int cloudBrightnessThreshold = medianBackgroundBrightness + (medianBackgroundBrightness - minBackgroundBrightness);

    qDebug() << "LAPLACE(min, max):" << QPointF(minLaplaceValue, maxLaplaceValue);
    qDebug() << "LAPLACE_THRESHOLD:" << laplace_threshold;
    qDebug() << "MIN_BACKGROUND_BRIGHTNESS:" << minBackgroundBrightness;
    qDebug() << "MAX_BACKGROUND_BRIGHTNESS:" << maxBackgroundBrightness;
    qDebug() << "MEDIAN_BACKGROUND_BRIGHTNESS:" << medianBackgroundBrightness;
    qDebug() << "CLOUD_BRIGHTNESS_THRESHOLD:" << cloudBrightnessThreshold;

//    cloudBrightnessThreshold = qMin(cloudBrightnessThreshold, 120);
//    cloudBrightnessThreshold = qMax(cloudBrightnessThreshold, 50);

    // PERFORM CLOUD'S MASK
    Mat cloudsMask(nSubRoiAlongY, nSubRoiAlongX, CV_8UC1);
    for (int r = 0; r < nSubRoiAlongY; r++)
    {
        for (int c = 0; c < nSubRoiAlongX; c++)
        {
            if (subRoiLaplaceValues.at<float>(r, c) < laplace_threshold && subRoiBrightnesses.at<uchar>(r, c) > cloudBrightnessThreshold) // && subRoiBrightnesses.at<uchar>(r, c) > maxBrightness
            {
                cloudsMask.at<uchar>(r, c) = 1;
            }
            else
            {
                cloudsMask.at<uchar>(r, c) = 0;
            }
        }
    }

    // Обнаружение односвязных областей (рекурсивный алгоритм)
    QStack<QPoint> cloudPixels;
    int cloudIndex = 1;
    for (int r = 0; r < nSubRoiAlongY; r++)
    {
        for (int c = 0; c < nSubRoiAlongX; c++)
        {
            if (cloudsMask.at<uchar>(r, c) == 1)
            {
                cloudIndex++;
                int nCloudPixels = 0;

                // Analyze pixel's neighbourhood
                cloudPixels.push(QPoint(c, r));
                while (!cloudPixels.empty())
                {
                    QPoint tmpCloudPixel = cloudPixels.pop();
                    nCloudPixels++;
                    cloudsMask.at<uchar>(tmpCloudPixel.y(), tmpCloudPixel.x()) = cloudIndex;
                    for (int i = tmpCloudPixel.y() - 1; i <= tmpCloudPixel.y() + 1; i++)
                    {
                        for (int j = tmpCloudPixel.x() - 1; j <= tmpCloudPixel.x() + 1; j++)
                        {
                            if (i >= 0 && i < nSubRoiAlongY && j >= 0 && j < nSubRoiAlongX)
                            {
                                if (cloudsMask.at<uchar>(i, j) == 1)
                                {
                                    cloudPixels.push(QPoint(j, i));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (cloudIndex > 1)
    {
        cloudsHaveBeenDetected = true;

        for (int r = 0; r < nSubRoiAlongY; r++)
        {
            for (int c = 0; c < nSubRoiAlongX; c++)
            {
                if (cloudsMask.at<uchar>(r, c) > 1)
                {
//                    if (subRoiBrightnesses.at<uchar>(r, c) > cloudBrightnessThreshold)
//                    {
//                        painter.fillRect(QRect(c * subRoiWidth, r * subRoiHeight, subRoiWidth, subRoiHeight), QColor(0, 128, 0, 255));
//                    }
//                    else
//                    {
//                        painter.fillRect(QRect(c * subRoiWidth, r * subRoiHeight, subRoiWidth, subRoiHeight), QColor(128, 0, 0, 255));
//                    }

                    painter.fillRect(QRect(c * subRoiWidth, r * subRoiHeight, subRoiWidth, subRoiHeight), QColor(0, 128, 0, 255));
                }
            }
        }
    }

    painter.end();

    return cloudsHaveBeenDetected;
}

void GLWidget::CROP_ROI()
{
    _cropRoi = true;
    update();
}

void GLWidget::increaseContrast(const Mat &srcImage, Mat &dstImage)
{
    float alpha = 1.75;
    float beta = 0.00;

    assert(srcImage.type() == CV_8UC1);
    assert((dstImage.type() == srcImage.type() && srcImage.rows == dstImage.rows && srcImage.cols == dstImage.cols) || !dstImage.data);

    if (!dstImage.data)
    {
        dstImage = Mat::zeros(srcImage.rows, srcImage.cols, srcImage.type());
    }

    for (int r = 0; r < srcImage.rows; r++)
    {
        for (int c = 0; c < srcImage.cols; c++)
        {
            dstImage.at<uchar>(r, c) = saturate_cast<uchar>( alpha * srcImage.at<uchar>(r, c) + beta );
        }
    }
}

void GLWidget::SHOW_CLOUDS(bool ok)
{
    if (ok)
    {
        _showClouds = true;
    }
    else
    {
        _showClouds = false;
    }
    update();
}

bool GLWidget::calcHistogram(const Mat& inputImage, Mat& hist, Mat &outputImage, int &minLoc, int &maxLoc){
    if (!inputImage.data){
        return false;
    }

    assert(inputImage.type() == CV_8UC1);

    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    Mat histGray;

    /// Compute the histograms:
    calcHist( &inputImage, 1, 0, Mat(), histGray, 1, &histSize, &histRange, uniform, accumulate );

    // Draw the histograms
    int hist_w = 256; int hist_h = 256;
    int bin_w = cvRound( (double) hist_w / histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {

        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(histGray.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(histGray.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );
    }

    outputImage = histImage.clone();

    /// Find max of histogram
    Point minloc, maxloc;
    double minval; double maxval;
    minMaxLoc(histGray, &minval, &maxval, &minloc, &maxloc);

    minLoc = minloc.y;
    maxLoc = maxloc.y;

    hist = histGray.clone();

    return true;
}

void GLWidget::bradleyThreshold(unsigned char* src, unsigned char* res, int width, int height) {
  const int S = width/8;
  int s2 = S/2;
  const float t = 0.15;
  unsigned long* integral_image = 0;
  long sum=0;
  int count=0;
  int index;
  int x1, y1, x2, y2;

  //рассчитываем интегральное изображение
  integral_image = new unsigned long [width*height*sizeof(unsigned long*)];

  for (int i = 0; i < width; i++) {
    sum = 0;
    for (int j = 0; j < height; j++) {
      index = j * width + i;
      sum += src[index];
      if (i==0)
    integral_image[index] = sum;
      else
    integral_image[index] = integral_image[index-1] + sum;
    }
  }

    //находим границы для локальные областей
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      index = j * width + i;

      x1=i-s2;
      x2=i+s2;
      y1=j-s2;
      y2=j+s2;

      if (x1 < 0)
    x1 = 0;
      if (x2 >= width)
    x2 = width-1;
      if (y1 < 0)
    y1 = 0;
      if (y2 >= height)
    y2 = height-1;

      count = (x2-x1)*(y2-y1);

      sum = integral_image[y2*width+x2] - integral_image[y1*width+x2] -
                  integral_image[y2*width+x1] + integral_image[y1*width+x1];
      if ((long)(src[index]*count) < (long)(sum*(1.0-t)))
    res[index] = 0;
      else
    res[index] = 255;
    }
  }

  delete[] integral_image;
}
