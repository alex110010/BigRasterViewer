#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>

// include GDAL headers
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "gdal.h"
#include "gdalwarper.h"

#include "gdal_utils.h"
#include "ogr_spatialref.h"
#include "cpl_string.h"
#include "ogr_geometry.h"
#include "ogrsf_frmts.h"

// include opencv headers
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:

private slots:
    void on_pB_openRasterData_clicked();
    void on_pB_openCacheFolder_clicked();

    void on_cB_showClouds_clicked();

    void on_pB_cropRoi_clicked();

    void on_pB_initViewer_clicked();

    void on_pB_detectCloudsInAllTiles_clicked();

    void on_pB_downloadRefSample_clicked();

    void on_pB_downloadCurSample_clicked();

    void on_pB_calculateCommonArea_clicked();

    void on_pB_calculateGeoShifts_clicked();

public slots:
    void SET_ZOOM(int newZoom);
signals:
    void INITIALIZE_OPENGL_VIEWER(int rasterWidth, int rasterHeight, QString rasterDataPath, QString rasterCachePath, int viewBoxSize, int subRoiSize);
    void SHOW_CLOUDS(bool ok);
    void CROP_ROI();
    void SET_SUBROI_SIZE(int subRoiSize);
private:
    Ui::MainWindow *ui;

    QString _cachePath;
    QString _rasterDataPath;

    bool _cachePathIsValid;
    bool _rasterDataPathIsValid;

    bool detectCloud(const Mat& curRoi, float c, const cv::Size& subRoiSize, QImage& qCurRoi, vector<QPoint>& cloudSubRoiCoords);
    QLabel* lbl_cloudsInSingleImage;

    uchar* cutRoi(GDALDataset* poDataSet, int x, int y, int width, int height, int bufWidth, int bufHeight);
    bool checkCvRoi(const Mat& cvRoi);

    // Модуль расчёта смещений
    void calculateCommomArea(GDALDataset* poRefDataSet, GDALDataset* poCurDataSet,
                             Point& refLeftPoint, int& refWidth, int& refHeight,
                             Point& curLeftPoint, int&  curWidth, int& curHeight,
                             double& scaleCoef, bool &success);

    QString _refSamplePath;
    QString _curSamplePath;

    GDALDataset* poRefDataSet;
    GDALDataset* poCurDataSet;

    Point _refLeftPoint;
    Point _curLeftPoint;
    int _refWidth;
    int _refHeight;
    int _curWidth;
    int _curHeight;
    float _scaleCoef;
    int _refTileWidth = 256;
    int _refTileHeight = 256;
    int _curTileWidth = 256;
    int _curTileHeight = 256;

    bool _commonAreaIsCalculated;

    // Correlation's functions
    void takeDFT(Mat& source, Mat& destination);
    void compareSpectrums(Mat &refDFT, Mat &curDFT, double l, Point &maxLoc, double &maxVal, double& corrContrast, bool deleteAxis, bool invertCorrMat);
    void recenterDFT(Mat &source);
};

#endif // MAINWINDOW_H
