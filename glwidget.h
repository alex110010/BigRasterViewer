#ifndef GLWiDGET_H
#define GLWiDGET_H

#include <QOpenGLWidget>
#include <QWheelEvent>
#include <QPaintEvent>
#include <QStack>
#include <QVector>
#include <QMessageBox>

// include GDAL headers
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "gdal.h"
#include "gdalwarper.h"

#include "gdal_utils.h"
#include "ogr_spatialref.h"
#include "cpl_string.h"
#include "ogr_geometry.h"

// include std
#include <assert.h>

// include opencv headers
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

//! [0]
class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent);
private:
    // Alex
    QPointF _currentCenterInWorld; // в ск мира
    float _rasterWidth;
    float _rasterHeight;

    QPointF _currentCenterInBase; // в ск подложки
    float _baseWidth;
    float _baseHeight;

    float _widgetWidth;
    float _widgetHeight;

    float _currentZoom;
    int _minZoom;
    int _maxZoom;
    QString _cachePath;
    QString _rasterDataPath;

    QString xyz2qrst(const QPoint& coords, int zoom);

    bool _mousePressed;
    QPointF _mousePressedCoords;


    bool detectCloud(const Mat& curRoi, float c, const cv::Size& subRoiSize, QImage& qCurRoi);
    void increaseContrast(const Mat& srcImage, Mat& dstImage);
    bool calcHistogram(const Mat& inputImage, Mat& hist, Mat &outputImage, int &minLoc, int &maxLoc);
    void bradleyThreshold(unsigned char* src, unsigned char* res, int width, int height);
    bool _showClouds;
    int _subRoiSize;

    bool _cropRoi;
public slots:
    void INITIALIZE_OPENGL_VIEWER(int rasterWidth, int rasterHeight, QString rasterDataPath, QString rasterCachePath, int viewBoxSize, int subRoiSize);
    void SHOW_CLOUDS(bool ok);
    void CROP_ROI();
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private slots:
signals:
    void SET_ZOOM(int newZoom);
};
#endif
