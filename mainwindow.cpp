#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMath>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug() << __FUNCTION__;
    ui->setupUi(this);
    connect(this, SIGNAL(INITIALIZE_OPENGL_VIEWER(int,int,QString,QString,int,int)), ui->openGLWidget, SLOT(INITIALIZE_OPENGL_VIEWER(int,int,QString,QString,int,int)));
    connect(ui->openGLWidget, SIGNAL(SET_ZOOM(int)), this, SLOT(SET_ZOOM(int)));
    connect(this, SIGNAL(SHOW_CLOUDS(bool)), ui->openGLWidget, SLOT(SHOW_CLOUDS(bool)));
    connect(this, SIGNAL(CROP_ROI()), ui->openGLWidget, SLOT(CROP_ROI()));

    _cachePath = QString::null;
    _rasterDataPath = QString::null;
    _cachePathIsValid = false;
    _rasterDataPathIsValid = false;

    ui->cB_tileSize->addItem("512x512", QVariant(512));
    ui->cB_tileSize->addItem("256x256", QVariant(256));
    ui->cB_tileSize->addItem("1024x1024", QVariant(1024));

    ui->openGLWidget->setMinimumSize(QSize(256, 256));
    ui->openGLWidget->setMaximumSize(QSize(1024, 1024));

    ui->openGLWidget->setFixedSize(QSize(ui->cB_tileSize->currentData().toInt(), ui->cB_tileSize->currentData().toInt()));

    ui->cB_subRoiSize->addItem("8x8", QVariant(8));
    ui->cB_subRoiSize->addItem("16x16", QVariant(16));
    ui->cB_subRoiSize->addItem("32x32", QVariant(32));
    ui->cB_subRoiSize->addItem("64x64", QVariant(64));

    ui->pB_initViewer->setStyleSheet("background-color: red; color: black");
    ui->pB_cropRoi->setStyleSheet("background-color: green; color: white");

    lbl_cloudsInSingleImage = nullptr;

    // Модуль расчёта смещений
    _refSamplePath = QString("");
    _curSamplePath = QString("");

    poRefDataSet = NULL;
    poCurDataSet = NULL;

    _commonAreaIsCalculated = false;
    _refLeftPoint = Point(0, 0);
    _refWidth = 0;
    _refHeight = 0;
    _curLeftPoint = Point(0, 0);
    _curWidth = 0;
    _curHeight = 0;
    _scaleCoef = 1;
}

MainWindow::~MainWindow()
{
    delete lbl_cloudsInSingleImage;
    delete ui;
}

void MainWindow::on_pB_openRasterData_clicked()
{
    QString rasterDataPath = QFileDialog::getOpenFileName(this, "Выберите растровое изображение", QApplication::applicationDirPath(), "Изображения (*.tif)");
    if (rasterDataPath.isEmpty() || rasterDataPath.isNull())
    {
        _rasterDataPathIsValid = false;
        return;
    }

    _rasterDataPath = rasterDataPath;
    _rasterDataPathIsValid = true;
    ui->le_rasterDataPath->setText(_rasterDataPath);
}

void MainWindow::on_pB_openCacheFolder_clicked()
{
    QString cachePath = QFileDialog::getExistingDirectory(this, "Выберите папку с тайловым кэшом",  QApplication::applicationDirPath());
    if (cachePath.isEmpty() || cachePath.isNull())
    {
        _cachePathIsValid = false;
        return;
    }
    _cachePath = cachePath;
    _cachePathIsValid = true;
    ui->le_cachePath->setText(_cachePath);
}

void MainWindow::SET_ZOOM(int newZoom)
{
    ui->le_zoom->setText(QString::number(newZoom));
}

void MainWindow::on_cB_showClouds_clicked()
{
    if (ui->cB_showClouds->isChecked())
    {
        emit SHOW_CLOUDS(true);
    }
    else
    {
        emit SHOW_CLOUDS(false);
    }
}

void MainWindow::on_pB_cropRoi_clicked()
{
    emit CROP_ROI();
}


void MainWindow::on_pB_initViewer_clicked()
{
    if (_cachePathIsValid && _rasterDataPathIsValid)
    {
        GDALAllRegister();
        GDALDataset* poRasterDataSet = (GDALDataset*)GDALOpen(_rasterDataPath.toStdString().data(), GA_ReadOnly);
        if (poRasterDataSet)
        {
            emit INITIALIZE_OPENGL_VIEWER(qMax(poRasterDataSet->GetRasterXSize(), poRasterDataSet->GetRasterYSize()), qMax(poRasterDataSet->GetRasterXSize(), poRasterDataSet->GetRasterYSize()),
                                          _rasterDataPath, _cachePath, ui->cB_tileSize->currentData().toInt(), ui->cB_subRoiSize->currentData().toInt());
        }
    }
}

void MainWindow::on_pB_detectCloudsInAllTiles_clicked()
{
    if (_rasterDataPathIsValid)
    {
        GDALAllRegister();
        GDALDataset* poDataSet = NULL;
        poDataSet = (GDALDataset*)GDALOpen(_rasterDataPath.toLocal8Bit().data(), GA_ReadOnly);
        if (poDataSet != NULL)
        {   
            double geoTransformParams[6];
            double pixelWidth;
            double pixelHeight;
            poDataSet->GetGeoTransform(geoTransformParams);
            pixelWidth = geoTransformParams[1];
            pixelHeight = geoTransformParams[5];

            int subRoiWidth;
            int subRoiHeight;
            if (ui->cB_useRecommendedSubRoiSize->isChecked())
            {
                subRoiWidth = 60 / pixelWidth;
                subRoiHeight = - 60 / pixelHeight;
                ui->sB_totalProcessing_subRoiSize->setValue(subRoiWidth);
            }
            else
            {
                subRoiWidth = ui->sB_totalProcessing_subRoiSize->value();
                subRoiHeight = subRoiWidth;
            }

            int roiWidth = ui->sB_analyzingWindowSize->value() * subRoiWidth;
            int roiHeight = ui->sB_analyzingWindowSize->value() * subRoiHeight;

            int roiLeftCorner_X = 0;
            int roiLeftCorner_Y = 0;

            int rasterWidth = poDataSet->GetRasterXSize();
            int rasterHeight = poDataSet->GetRasterYSize();

            // Initial progress bar
            int nRoisAlongX = rasterWidth / roiWidth;
            int nRoisAlongY = rasterHeight / roiHeight;
            ui->progressBar_detectClouds->setMinimum(0);
            ui->progressBar_detectClouds->setMaximum(nRoisAlongX * nRoisAlongY);
            ui->progressBar_detectClouds->setValue(0);
            ui->gB_totalProcessing->setEnabled(false);

            QString pathToTrash("");
            QStringList parsedRasterDataPath = _rasterDataPath.split("/");
            if (!parsedRasterDataPath.isEmpty())
            {
                for (int i = 0; i < parsedRasterDataPath.size() - 1; i++) pathToTrash += parsedRasterDataPath[i] + "/";
                pathToTrash += "trash_";
                int numOfTrashFolder = 0;
                while (QDir(pathToTrash + QString::number(numOfTrashFolder)).exists())
                {
                    numOfTrashFolder++;
                }
                pathToTrash += QString::number(numOfTrashFolder);
                mkdir(pathToTrash.toLocal8Bit().data());
            }
            qDebug() << pathToTrash;

            int nProcessedRois = 0;

            // GDAL *.shp
            const char *pszDriverName = "ESRI Shapefile";
            GDALDriver *poDriver;
            poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName );
            if ( poDriver == NULL )
            {
                printf( "%s driver not available.\n", pszDriverName );
                return;
            }

            QString shpPath = pathToTrash + "/coords.shp";
            GDALDataset *poShapeDataSet = poDriver->Create(shpPath.toStdString().data(), 0, 0, 0, GDT_Unknown, NULL );

            OGRLayer *poLayerPolygonArea;
            poLayerPolygonArea = poShapeDataSet->CreateLayer("polygon_area", NULL, wkbPolygon, NULL);

            OGRFieldDefn id("id_name", OFTInteger);
            id.SetWidth(10);
            poLayerPolygonArea->CreateField(&id);

            char *pszWKT;
            OGRSpatialReference* proj = new OGRSpatialReference(poDataSet->GetProjectionRef());
            proj->exportToWkt(&pszWKT);

            QFile fileProj( QFileInfo(shpPath).path() + "/" + QFileInfo(shpPath).completeBaseName() + ".prj");
            fileProj.open(QIODevice::WriteOnly);
            fileProj.write(pszWKT);
            fileProj.close();

            while (roiLeftCorner_Y + roiHeight < rasterHeight)
            {
                while (roiLeftCorner_X + roiWidth < rasterWidth)
                {
                    qDebug() << nProcessedRois;
                    uchar* buf = cutRoi(poDataSet, roiLeftCorner_X, roiLeftCorner_Y, roiWidth, roiHeight, roiWidth, roiHeight);

                    if (buf != nullptr)
                    {
                        Mat cvRoi(roiWidth, roiHeight, CV_8UC1, (uchar*)buf);

                        // check cvRoi
                        bool roiIsChecked = checkCvRoi(cvRoi);

                        if (roiIsChecked)
                        {
                            uchar* buf_copy = new uchar[roiWidth * roiHeight];
                            for (int i = 0; i < roiWidth * roiHeight; i++) buf_copy[i] = buf[i];

                            QImage qRoi((uchar*)buf_copy , roiWidth, roiHeight, QImage::Format_Grayscale8);
                            QImage qColorRoi = qRoi.convertToFormat(QImage::Format_RGB888).copy();

                            vector<QPoint> cloudSubRoiCoords;
                            // Preprocessing
                            if (ui->cB_useGammaCorrection)
                            {
                                Mat lookUpTable(1, 256, CV_8U);
                                uchar* p = lookUpTable.ptr();
                                double gamma_ = ui->sB_gammaCorrectionValue->value();
                                for (int i = 0; i < 256; ++i) p[i] = saturate_cast<uchar>(qPow(i / 255.0, gamma_) * 255.0);
                                Mat cvEqualizedRoi = cvRoi.clone();
                                LUT(cvRoi, lookUpTable, cvEqualizedRoi);

                                bool success = detectCloud(cvEqualizedRoi, ui->sB_laplaceThreshold->value(), Size(subRoiWidth, subRoiHeight), qColorRoi, cloudSubRoiCoords);
                                qDebug() << success;
                            }
                            else
                            {
                                bool success = detectCloud(cvRoi, 0.4, Size(subRoiWidth, subRoiHeight), qColorRoi, cloudSubRoiCoords);
                                qDebug() << success;
                            }

                            if (!cloudSubRoiCoords.empty())
                            {
                                for (int i = 0; i < cloudSubRoiCoords.size(); i++)
                                {
                                    cloudSubRoiCoords[i].setX(cloudSubRoiCoords[i].x() + roiLeftCorner_X);
                                    cloudSubRoiCoords[i].setY(cloudSubRoiCoords[i].y() + roiLeftCorner_Y);

                                    OGRFeature *poFeaturePolygonArea;
                                    poFeaturePolygonArea = OGRFeature::CreateFeature( poLayerPolygonArea->GetLayerDefn() );

                                    OGRPolygon polygonArea;
                                    OGRLinearRing ogrRing;

                                    OGRPoint ptArea_Tl;
                                    ptArea_Tl.setX(cloudSubRoiCoords[i].x());
                                    ptArea_Tl.setY(cloudSubRoiCoords[i].y());
                                    ptArea_Tl.setX(geoTransformParams[0] + ptArea_Tl.getX() * geoTransformParams[1] + ptArea_Tl.getY() * geoTransformParams[2]);
                                    ptArea_Tl.setY(geoTransformParams[3] + ptArea_Tl.getX() * geoTransformParams[4] + ptArea_Tl.getY() * geoTransformParams[5]);

                                    OGRPoint ptArea_Tr;
                                    ptArea_Tr.setX(cloudSubRoiCoords[i].x() + subRoiWidth);
                                    ptArea_Tr.setY(cloudSubRoiCoords[i].y());
                                    ptArea_Tr.setX(geoTransformParams[0] + ptArea_Tr.getX() * geoTransformParams[1] + ptArea_Tr.getY() * geoTransformParams[2]);
                                    ptArea_Tr.setY(geoTransformParams[3] + ptArea_Tr.getX() * geoTransformParams[4] + ptArea_Tr.getY() * geoTransformParams[5]);

                                    OGRPoint ptArea_Br;
                                    ptArea_Br.setX(cloudSubRoiCoords[i].x() + subRoiWidth);
                                    ptArea_Br.setY(cloudSubRoiCoords[i].y() + subRoiHeight);
                                    ptArea_Br.setX(geoTransformParams[0] + ptArea_Br.getX() * geoTransformParams[1] + ptArea_Br.getY() * geoTransformParams[2]);
                                    ptArea_Br.setY(geoTransformParams[3] + ptArea_Br.getX() * geoTransformParams[4] + ptArea_Br.getY() * geoTransformParams[5]);

                                    OGRPoint ptArea_Bl;
                                    ptArea_Bl.setX(cloudSubRoiCoords[i].x());
                                    ptArea_Bl.setY(cloudSubRoiCoords[i].y() + subRoiHeight);
                                    ptArea_Bl.setX(geoTransformParams[0] + ptArea_Bl.getX() * geoTransformParams[1] + ptArea_Bl.getY() * geoTransformParams[2]);
                                    ptArea_Bl.setY(geoTransformParams[3] + ptArea_Bl.getX() * geoTransformParams[4] + ptArea_Bl.getY() * geoTransformParams[5]);

                                    ogrRing.setPoint(0, &ptArea_Tl);
                                    ogrRing.setPoint(1, &ptArea_Tr);
                                    ogrRing.setPoint(2, &ptArea_Br);
                                    ogrRing.setPoint(3, &ptArea_Bl);
                                    ogrRing.setPoint(4, &ptArea_Tl);

                                    polygonArea.addRing(&ogrRing);

                                    poFeaturePolygonArea->SetGeometry(&polygonArea);
                                    poLayerPolygonArea->CreateFeature(poFeaturePolygonArea);
                                    OGRFeature::DestroyFeature(poFeaturePolygonArea);
                                }
                            }

//                            qColorRoi.save(pathToTrash + "/" + QString::number(nProcessedRois) + ".png");
                            delete[] (uchar*) buf_copy;
                        }

                        delete [] (uchar*)buf;
                    }

                    roiLeftCorner_X += roiWidth;
                    nProcessedRois++;
                    ui->progressBar_detectClouds->setValue(ui->progressBar_detectClouds->value() + 1);
                    QApplication::processEvents();
                }

                roiLeftCorner_Y += roiHeight;
                roiLeftCorner_X = 0;
            }

            GDALClose((GDALDatasetH)poShapeDataSet);
            GDALClose((GDALDatasetH)poDataSet);

            QMessageBox::information(this, "Успех", "Изображение успешно обработано");
            ui->progressBar_detectClouds->setValue(0);
            ui->gB_totalProcessing->setEnabled(true);
        }
    }
}

bool MainWindow::detectCloud(const Mat &curRoi, float c, const cv::Size &subRoiSize, QImage &qCurRoi, vector<QPoint> &cloudSubRoiCoords)
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

    // Perform background brightnesses
    QVector<int> backgroundBrightnesses;
    for (int r = 0; r < nSubRoiAlongY; r++)
    {
        for (int c = 0; c < nSubRoiAlongX; c++)
        {
            if (subRoiLaplaceValues.at<float>(r, c) > laplace_threshold) backgroundBrightnesses.push_back(subRoiBrightnesses.at<uchar>(r, c));
        }
    }

    // Sort background brightnesses (vector<uchar>)
    int minBackgroundBrightness = 0;
    int medianBackgroundBrightness = 0;
    int cloudBrightnessThreshold = 0;
    int maxBackgroundBrightness = 0;
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

        minBackgroundBrightness = backgroundBrightnesses[0];
        maxBackgroundBrightness = backgroundBrightnesses[backgroundBrightnesses.size() - 1];
        medianBackgroundBrightness = backgroundBrightnesses[backgroundBrightnesses.size() / 2];
        cloudBrightnessThreshold = medianBackgroundBrightness + (medianBackgroundBrightness - minBackgroundBrightness);
        cloudBrightnessThreshold = qBound(ui->sB_minBrightness->value(), cloudBrightnessThreshold, ui->sB_maxBrightness->value());

        /*
        if (backgroundBrightnesses.size() < (nSubRoiAlongX * nSubRoiAlongY) / 4)
        {
            qDebug() << "if_1";
            if (predefinedBrightnessThreshold > 0) cloudBrightnessThreshold = predefinedBrightnessThreshold;
            else
            {
                maxBackgroundBrightness = -1;
                return false;
            }
        }
        else if (backgroundBrightnesses.size() > (nSubRoiAlongX * nSubRoiAlongY) * 3 / 4)
        {
            qDebug() << "if_2";
            predefinedBrightnessThreshold = maxBackgroundBrightness;
            predefinedBrightnessThresholdWasChanged = true;

            medianBackgroundBrightness = backgroundBrightnesses[backgroundBrightnesses.size() / 2];
            cloudBrightnessThreshold = medianBackgroundBrightness + (medianBackgroundBrightness - minBackgroundBrightness);
        }
        else
        {
            qDebug() << "if_3";
            medianBackgroundBrightness = backgroundBrightnesses[backgroundBrightnesses.size() / 2];
            cloudBrightnessThreshold = predefinedBrightnessThreshold;
        }
        */
    }
    else
    {
        /*
        if (predefinedBrightnessThreshold > 0) cloudBrightnessThreshold = predefinedBrightnessThreshold;
        else
        {
            maxBackgroundBrightness = -1;
            return false;
        }
        */

        cloudBrightnessThreshold = 128;
    }

    qDebug() << "MIN_BACKGROUND_BRIGHTNESS:" << minBackgroundBrightness;
    qDebug() << "MAX_BACKGROUND_BRIGHTNESS:" << maxBackgroundBrightness;
    qDebug() << "MEDIAN_BACKGROUND_BRIGHTNESS:" << medianBackgroundBrightness;
    qDebug() << "CLOUD_BRIGHTNESS_THRESHOLD:" << cloudBrightnessThreshold;

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

    for (int r = 0; r < nSubRoiAlongY; r++)
    {
        for (int c = 0; c < nSubRoiAlongX; c++)
        {
            if (cloudsMask.at<uchar>(r, c) == 1)
            {
                painter.fillRect(QRect(c * subRoiWidth, r * subRoiHeight, subRoiWidth, subRoiHeight), QColor(191, 239, 255, 255));
                cloudSubRoiCoords.push_back(QPoint(c * subRoiWidth, r * subRoiHeight));
                cloudsHaveBeenDetected = true;
            }
        }
    }

    painter.end();

    return cloudsHaveBeenDetected;
}

uchar* MainWindow::cutRoi(GDALDataset *poDataSet, int x, int y, int width, int height, int bufWidth, int bufHeight)
{
    if (poDataSet == NULL)
    {
        QMessageBox::information(this, "Ошибка", "Доступ к изображению не был получен!");
        return nullptr;
    }

    if (width == 0 || height == 0)
    {
        QMessageBox::information(this, "Ошибка", "Размеры вырезаемой области изображения не могут быть нулевыми!");
        return nullptr;
    }

    if (x > poDataSet->GetRasterXSize() || x < 0 ||
            x + width > poDataSet->GetRasterXSize() || x + width < 0 ||
            y > poDataSet->GetRasterYSize() || y < 0 ||
            y + height > poDataSet->GetRasterYSize() || y + height < 0)
    {
        QMessageBox::information(this, "Ошибка", "Вырезаемая область вышла за пределы изображения!");
        return nullptr;
    }
    uchar* buf = new uchar[bufWidth * bufHeight];

    poDataSet->GetRasterBand(1)->RasterIO(GF_Read, x, y, width, height, buf, bufWidth, bufHeight, GDT_Byte, 0, 0); // Данные первого канала

    return buf;
}

bool MainWindow::checkCvRoi(const Mat &cvRoi)
{
    int nBlackPixels = 0;

    for (int c = 0; c < cvRoi.cols; c++)
    {
        if (cvRoi.at<uchar>(0, c) == 0) nBlackPixels++;
    }

    for (int r = 1; r < cvRoi.rows; r++)
    {
        if (cvRoi.at<uchar>(0, cvRoi.cols - 1) == 0) nBlackPixels++;
    }

    for (int c = cvRoi.cols - 2; c >= 0; c--)
    {
        if (cvRoi.at<uchar>(cvRoi.rows - 1, c) == 0) nBlackPixels++;
    }

    for (int r = cvRoi.rows - 2; r > 0; r--)
    {
        if (cvRoi.at<uchar>(r, 0) == 0) nBlackPixels++;
    }

    int cvRoiPerimeter = (cvRoi.cols + cvRoi.rows) / 2;
    if (nBlackPixels > cvRoiPerimeter / 5) return false;
    else return true;
}

void MainWindow::on_pB_downloadRefSample_clicked()
{
    QString samplePath = QFileDialog::getOpenFileName(this, "Выбор опорного эталонного изображения", QApplication::applicationDirPath(), "Изображения (*.bmp *.jpg *.jpeg *.png *.tif *.tiff)");
    if (samplePath.isEmpty()){
        if (_refSamplePath.isEmpty()) ui->le_refSamplePath->setText("sample wasn't downloaded");
        return;
    }

    // save and show sample path
    ui->le_refSamplePath->setText(samplePath);
    this->_refSamplePath = samplePath;

    // Read sample's options
    GDALAllRegister();
    poRefDataSet = (GDALDataset*)GDALOpen(samplePath.toStdString().data(), GA_ReadOnly);
    if (poRefDataSet == NULL){
        qDebug() << "GDAL_REF_DATA_SET = NULL!";
        return;
    }

    ui->le_refSampleNChannels->setText(QString::number(poRefDataSet->GetRasterCount()));
    ui->le_refSampleWidth->setText(QString::number(poRefDataSet->GetRasterXSize()));
    ui->le_refSampleHeight->setText(QString::number(poRefDataSet->GetRasterYSize()));
}

void MainWindow::on_pB_downloadCurSample_clicked()
{
    QString samplePath = QFileDialog::getOpenFileName(this, "Выбор текущего эталонного изображения", QApplication::applicationDirPath(), "Изображения (*.bmp *.jpg *.jpeg *.png *.tif *.tiff)");
    if (samplePath.isEmpty()){
        if (_curSamplePath.isEmpty()) ui->le_curSamplePath->setText("sample wasn't downloaded");
        return;
    }

    // save and show sample path
    ui->le_curSamplePath->setText(samplePath);
    this->_curSamplePath = samplePath;

    // Read sample's options
    GDALAllRegister();
    poCurDataSet = (GDALDataset*)GDALOpen(samplePath.toStdString().data(), GA_ReadOnly);
    if (poCurDataSet == NULL){
        qDebug() << "GDAL_CUR_DATA_SET = NULL!";
        return;
    }

    ui->le_curSampleNChannels->setText(QString::number(poCurDataSet->GetRasterCount()));
    ui->le_curSampleWidth->setText(QString::number(poCurDataSet->GetRasterXSize()));
    ui->le_curSampleHeight->setText(QString::number(poCurDataSet->GetRasterYSize()));
}

void MainWindow::on_pB_calculateCommonArea_clicked()
{
    _commonAreaIsCalculated = false;

    Point refLeftPoint = Point();
    Point curLeftPoint = Point();
    int refWidth;
    int refHeight;
    int curWidth;
    int curHeight;
    double scaleCoef;
    bool success = false;

    if (poRefDataSet == NULL || poCurDataSet == NULL)
    {
        QMessageBox::information(this, "Ошибка", "Доступ к изображениям не был получен!");
        return;
    }

    calculateCommomArea(poRefDataSet, poCurDataSet, refLeftPoint, refWidth, refHeight, curLeftPoint, curWidth, curHeight, scaleCoef, success);

    if (success)
    {
        _commonAreaIsCalculated = true;

        _refLeftPoint = refLeftPoint;
        _refWidth = refWidth;
        _refHeight = refHeight;

        _curLeftPoint = curLeftPoint;
        _curWidth = curWidth;
        _curHeight = curHeight;

        _scaleCoef = scaleCoef;

        // Calculate topleft corners of intersection's tiles
        _refTileWidth = ui->sB_calculateGeoShifts_analyzingWindowSize->value();
        _refTileHeight = ui->sB_calculateGeoShifts_analyzingWindowSize->value();

        _curTileWidth = _refTileWidth * qRound(_scaleCoef);
        _curTileHeight = _refTileHeight * qRound(_scaleCoef);

        int refNXStep = _refWidth / _refTileWidth;
        int refNYStep = _refHeight / _refTileHeight;
        int curNXStep = _curWidth / _curTileWidth;
        int curNYStep = _curHeight / _curTileHeight;

        qDebug() << "REF:" << _refLeftPoint.x << _refLeftPoint.y << _refWidth << refHeight;
        qDebug() << "CUR:" << _curLeftPoint.x << _curLeftPoint.y << _curWidth << _curHeight;
        qDebug() << "SCALE_COEF:" << _scaleCoef;
        qDebug() << "REF_TILE:" << QSize(_refTileWidth, _refTileHeight);
        qDebug() << "CUR_TILE:" << QSize(_curTileWidth, _curTileHeight);
        qDebug() << "REF_STEP:" << QSize(refNXStep, refNYStep);
        qDebug() << "CUR_STEP:" << QSize(curNXStep, curNYStep);

        QMessageBox::information(this, "Успех!", "Геодезические координаты общей области рассчитаны!");
    }
    else
    {
        if (!_commonAreaIsCalculated)
        {
            _refLeftPoint = Point(0, 0);
            _refWidth = 0;
            _refHeight = 0;
            _curLeftPoint = Point(0, 0);
            _curWidth = 0;
            _curHeight = 0;
            _scaleCoef = 1;
        }
        QMessageBox::information(this, "Ошибка!", "Геодезические координаты не вычислены!");
    }
}

void MainWindow::calculateCommomArea(GDALDataset *poRefDataSet, GDALDataset *poCurDataSet, Point &refLeftPoint, int &refWidth, int &refHeight, Point &curLeftPoint, int &curWidth, int &curHeight, double &scaleCoef, bool &success)
{
    OGRSpatialReference *spRef = new OGRSpatialReference(poRefDataSet->GetProjectionRef());
    OGRSpatialReference *spCur = new OGRSpatialReference(poCurDataSet->GetProjectionRef());

    double adfGeoTransformRef[6], adfGeoTransformCur[6];
    GDALGetGeoTransform(poRefDataSet, adfGeoTransformRef);
    GDALGetGeoTransform(poCurDataSet, adfGeoTransformCur);

    int wRef = GDALGetRasterXSize(poRefDataSet)-1;
    int hRef = GDALGetRasterYSize(poRefDataSet)-1;
    int wCur = GDALGetRasterXSize(poCurDataSet)-1;
    int hCur = GDALGetRasterYSize(poCurDataSet)-1;

    QRectF rectRef(0, 0, wRef, hRef);

    double xTLcur = adfGeoTransformCur[0];
    double yTLcur = adfGeoTransformCur[3];

    double xBRcur = adfGeoTransformCur[0] + (wCur)*adfGeoTransformCur[1] + (hCur)*adfGeoTransformCur[2];
    double yBRcur = adfGeoTransformCur[3] + (wCur)*adfGeoTransformCur[4] + (hCur)*adfGeoTransformCur[5];

    OGRCoordinateTransformation *poTransformCurToRef = OGRCreateCoordinateTransformation(spCur, spRef);
    poTransformCurToRef->Transform(1, &xTLcur, &yTLcur);
    poTransformCurToRef->Transform(1, &xBRcur, &yBRcur);
    OGRCoordinateTransformation::DestroyCT(poTransformCurToRef);

    double _yTLcur = (adfGeoTransformRef[1] * (yTLcur - adfGeoTransformRef[3]) - adfGeoTransformRef[4] * (xTLcur - adfGeoTransformRef[0])) / (adfGeoTransformRef[1] * adfGeoTransformRef[5] - adfGeoTransformRef[4]*adfGeoTransformRef[2]);
    double _xTLcur = (xTLcur - adfGeoTransformRef[0] - adfGeoTransformRef[2] * _yTLcur) / adfGeoTransformRef[1];

    double _yBRcur = (adfGeoTransformRef[1] * (yBRcur - adfGeoTransformRef[3]) - adfGeoTransformRef[4] * (xBRcur - adfGeoTransformRef[0])) / (adfGeoTransformRef[1] * adfGeoTransformRef[5] - adfGeoTransformRef[4]*adfGeoTransformRef[2]);
    double _xBRcur = (xBRcur - adfGeoTransformRef[0] - adfGeoTransformRef[2] * _yBRcur) / adfGeoTransformRef[1];

    QRectF rectCur;
    rectCur.setLeft(_xTLcur);
    rectCur.setTop(_yTLcur);
    rectCur.setRight(_xBRcur);
    rectCur.setBottom(_yBRcur);

    QRectF rectCommon = rectRef.intersected(rectCur);
    success = rectCommon.isValid();

    refLeftPoint.x = qCeil(rectCommon.left());
    refLeftPoint.y = qCeil(rectCommon.top());
    refWidth = qFloor(rectCommon.width());
    refHeight = qFloor(rectCommon.height());

    //Пересчитаем rectCommon в СК poCurDataSet

    double xTLref = adfGeoTransformRef[0] + rectCommon.left()*adfGeoTransformRef[1] + rectCommon.top()*adfGeoTransformRef[2];
    double yTLref = adfGeoTransformRef[3] + rectCommon.left()*adfGeoTransformRef[4] + rectCommon.top()*adfGeoTransformRef[5];

    double xBRref = adfGeoTransformRef[0] + rectCommon.right()*adfGeoTransformRef[1] + rectCommon.bottom()*adfGeoTransformRef[2];
    double yBRref = adfGeoTransformRef[3] + rectCommon.right()*adfGeoTransformRef[4] + rectCommon.bottom()*adfGeoTransformRef[5];

    OGRCoordinateTransformation *poTransformRefToCur = OGRCreateCoordinateTransformation(spRef, spCur);
    poTransformRefToCur->Transform(1, &xTLref, &yTLref);
    poTransformRefToCur->Transform(1, &xBRref, &yBRref);
    OGRCoordinateTransformation::DestroyCT(poTransformRefToCur);

    double _yTLref = (adfGeoTransformCur[1] * (yTLref - adfGeoTransformCur[3]) - adfGeoTransformCur[4] * (xTLref - adfGeoTransformCur[0])) / (adfGeoTransformCur[1] * adfGeoTransformCur[5] - adfGeoTransformCur[4]*adfGeoTransformCur[2]);
    double _xTLref = (xTLref - adfGeoTransformCur[0] - adfGeoTransformCur[2] * _yTLref) / adfGeoTransformCur[1];

    double _yBRref = (adfGeoTransformCur[1] * (yBRref - adfGeoTransformCur[3]) - adfGeoTransformCur[4] * (xBRref - adfGeoTransformCur[0])) / (adfGeoTransformCur[1] * adfGeoTransformCur[5] - adfGeoTransformCur[4]*adfGeoTransformCur[2]);
    double _xBRref = (xBRref - adfGeoTransformCur[0] - adfGeoTransformCur[2] * _yBRref) / adfGeoTransformCur[1];

     qDebug() << rectRef << rectCur << rectCommon << success;

     qDebug() << _xTLref << _yTLref;

    curLeftPoint.x = qCeil(_xTLref);
    curLeftPoint.y = qCeil(_yTLref);
    curWidth = qFloor(_xBRref - _xTLref);
    curHeight = qFloor(_yBRref - _yTLref);

    scaleCoef = (float)curWidth / (float)refWidth;
}

void MainWindow::on_pB_calculateGeoShifts_clicked()
{
    int refNXStep = _refWidth / _refTileWidth;
    int refNYStep = _refHeight / _refTileHeight;

    // Initial progress bar
    ui->progressBar_calculateGeoShifts->setMinimum(0);
    ui->progressBar_calculateGeoShifts->setMaximum(refNXStep * refNYStep);
    ui->pB_calculateCommonArea->setEnabled(false);
    ui->pB_calculateGeoShifts->setEnabled(false);
    ui->progressBar_calculateGeoShifts->setValue(0);

    // GDAL *.shp
    const char *pszDriverName = "ESRI Shapefile";
    GDALDriver *poDriver;
    poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName );
    if ( poDriver == NULL )
    {
        printf( "%s driver not available.\n", pszDriverName );
        return;
    }

    QString shpPath = QCoreApplication::applicationDirPath() + "/Geo_shifts";
    if (!QDir(shpPath).exists()) mkdir(shpPath.toLocal8Bit().data());
    shpPath += "/geo_shifts.shp";

    GDALDataset *poShapeDataSet = poDriver->Create(shpPath.toStdString().data(), 0, 0, 0, GDT_Unknown, NULL );

    OGRLayer *poLayerPoint;
    poLayerPoint = poShapeDataSet->CreateLayer("point", NULL, wkbPoint, NULL);

    OGRLayer *poLayerLine;
    poLayerLine = poShapeDataSet->CreateLayer("line", NULL, wkbLineString, NULL);

    OGRLayer *poLayerLine_Scope;
    poLayerLine_Scope = poShapeDataSet->CreateLayer("line_scope", NULL, wkbLineString, NULL);

    OGRLayer *poLayerPolygon_Area;
    poLayerPolygon_Area = poShapeDataSet->CreateLayer("polygon_area", NULL, wkbPolygon, NULL);

    OGRLayer *poLayerLine_Area;
    poLayerLine_Area = poShapeDataSet->CreateLayer("line_area", NULL, wkbLineString, NULL);

    OGRFieldDefn id("id_name", OFTInteger);

    id.SetWidth(10);
    poLayerPoint->CreateField(&id);
    poLayerLine->CreateField(&id);
    poLayerLine_Scope->CreateField(&id);
    poLayerLine_Area->CreateField(&id);
    poLayerPolygon_Area->CreateField(&id);

    OGRFeature *poFeature_Area;
    poFeature_Area = OGRFeature::CreateFeature( poLayerLine_Area->GetLayerDefn() );

    char *pszWKT;
    OGRSpatialReference* proj = new OGRSpatialReference(poRefDataSet->GetProjectionRef());
    proj->exportToWkt(&pszWKT);

    QFile fileProj( QFileInfo(shpPath).path() + "/" + QFileInfo(shpPath).completeBaseName() + ".prj");
    fileProj.open(QIODevice::WriteOnly);
    fileProj.write(pszWKT);
    fileProj.close();

    QFile file_Proj_point( QFileInfo(shpPath).path() + "/" +  "point.prj");
    file_Proj_point.open(QIODevice::WriteOnly);
    proj->exportToWkt(&pszWKT);
    file_Proj_point.write(pszWKT);
    file_Proj_point.close();

    QFile file_Proj_line( QFileInfo(shpPath).path() + "/" + "line.prj");
    file_Proj_line.open(QIODevice::WriteOnly);
    proj->exportToWkt(&pszWKT);
    file_Proj_line.write(pszWKT);
    file_Proj_line.close();

    QFile file_Proj_line_Scope( QFileInfo(shpPath).path() + "/" + "line_scope.prj");
    file_Proj_line_Scope.open(QIODevice::WriteOnly);
    proj->exportToWkt(&pszWKT);
    file_Proj_line_Scope.write(pszWKT);
    file_Proj_line_Scope.close();

    QFile file_Proj_polygon_area( QFileInfo(shpPath).path() + "/" + "polygon_area.prj");
    file_Proj_polygon_area.open(QIODevice::WriteOnly);
    proj->exportToWkt(&pszWKT);
    file_Proj_polygon_area.write(pszWKT);
    file_Proj_polygon_area.close();

    QFile file_Proj_line_area( QFileInfo(shpPath).path() + "/" + "line_area.prj");
    file_Proj_line_area.open(QIODevice::WriteOnly);
    proj->exportToWkt(&pszWKT);
    file_Proj_line_area.write(pszWKT);
    file_Proj_line_area.close();

    QPolygonF qScope;
    int idCount = 0;

    for (int r = 0; r < refNYStep; r++)
    {
        for (int c = 0; c < refNXStep; c++)
        {
            qDebug() << idCount;
            idCount++;

            Point refPt = Point(_refLeftPoint.x + c * _refTileWidth, _refLeftPoint.y + r * _refTileHeight);
            Point curPt = Point(_curLeftPoint.x + c * _curTileWidth, _curLeftPoint.y + r * _curTileHeight);

            GDALDataset* poDataSet;

            // FOR REFERENCE SAMPLE
            poDataSet = poRefDataSet;

            void* refBuf;
            refBuf = cutRoi(poDataSet, refPt.x, refPt.y, _refTileWidth, _refTileHeight, _refTileWidth, _refTileHeight); // return nullptr или uchar*

            if (refBuf == nullptr) return;

            // FOR CURRENT SAMPLE
            poDataSet = poCurDataSet;

            void* curBuf;
            curBuf = cutRoi(poDataSet, curPt.x, curPt.y, _curTileWidth, _curTileHeight, _curTileWidth, _curTileHeight); // return nullptr или uchar*

            if (curBuf == nullptr) return;

            // call correlation
            Point maxLoc;
            double maxVal;
            double corrContrast;
            bool deleteAxis = false;
            bool invertCorrMat = false;

            Mat refMat(_refTileHeight, _refTileWidth, CV_8UC1, refBuf);
            Mat curMat(_refTileHeight, _refTileWidth, CV_8UC1, curBuf);

            if (!checkCvRoi(refMat) || !(checkCvRoi(curMat)))
            {
                // Release image's data
                delete [] (uchar*)refBuf;
                delete [] (uchar*)curBuf;

                ui->progressBar_calculateGeoShifts->setValue(ui->progressBar_calculateGeoShifts->value() + 1);
                QApplication::processEvents();

                continue;
            }

            // Scale curMat
            Mat scaledCurMat;
            cv::resize(curMat, scaledCurMat, Size(refMat.cols, refMat.rows));

            Mat refMatAfterFloat;
            refMat.convertTo(refMatAfterFloat, CV_32FC1, 1.0 / 255.0);
            Mat dftRefMatAfter;
            takeDFT(refMatAfterFloat, dftRefMatAfter);

            Mat curMatAfterFloat;
            scaledCurMat.convertTo(curMatAfterFloat, CV_32FC1, 1.0 / 255.0);
            Mat dftCurMatAfter;
            takeDFT(curMatAfterFloat, dftCurMatAfter);

            // Calculate correlation
            compareSpectrums(dftRefMatAfter, dftCurMatAfter, 0.3, maxLoc, maxVal, corrContrast, deleteAxis, invertCorrMat);
            if (corrContrast < 7)
            {
                // Release image's data
                delete [] (uchar*)refBuf;
                delete [] (uchar*)curBuf;

                continue;
            }

            // GDAL
            double geoTransformParams[6];
            double pixelWidth;
            double pixelHeight;
            poRefDataSet->GetGeoTransform(geoTransformParams);
            pixelWidth = geoTransformParams[1];
            pixelHeight = geoTransformParams[5];

            double toMetr_Polygon_Tl_X = geoTransformParams[0] + refPt.x  * geoTransformParams[1] + refPt.y  * geoTransformParams[2];
            double toMetr_Polygon_Tl_Y = geoTransformParams[3] + refPt.x  * geoTransformParams[4] + refPt.y  * geoTransformParams[5];
            double toMetr_Polygon_Tr_X = geoTransformParams[0] + (refPt.x + _refTileWidth)  * geoTransformParams[1] + refPt.y * geoTransformParams[2];
            double toMetr_Polygon_Tr_Y = geoTransformParams[3] + (refPt.x + _refTileWidth)  * geoTransformParams[4] + refPt.y * geoTransformParams[5];
            double toMetr_Polygon_Br_X = geoTransformParams[0] + (refPt.x + + _refTileHeight) * geoTransformParams[1] + (refPt.y + _refTileHeight ) * geoTransformParams[2];
            double toMetr_Polygon_Br_Y = geoTransformParams[3] + (refPt.x + + _refTileHeight)  * geoTransformParams[4] + (refPt.y + _refTileHeight ) * geoTransformParams[5];
            double toMetr_Polygon_Bl_X = geoTransformParams[0] + refPt.x  * geoTransformParams[1] + (refPt.y + _refTileHeight ) * geoTransformParams[2];
            double toMetr_Polygon_Bl_Y = geoTransformParams[3] + refPt.x  * geoTransformParams[4] + (refPt.y + _refTileHeight ) * geoTransformParams[5];

            qScope.clear();
            qScope << QPointF(toMetr_Polygon_Tl_X, toMetr_Polygon_Tl_Y);
            qScope << QPointF(toMetr_Polygon_Tr_X, toMetr_Polygon_Tr_Y);
            qScope << QPointF(toMetr_Polygon_Br_X, toMetr_Polygon_Br_Y);
            qScope << QPointF(toMetr_Polygon_Bl_X, toMetr_Polygon_Bl_Y);

            QPointF shiftVectorStart((toMetr_Polygon_Tr_X + toMetr_Polygon_Tl_X) / 2, (toMetr_Polygon_Tl_Y + toMetr_Polygon_Bl_Y) / 2);
            QPointF shiftVectorEnd(toMetr_Polygon_Tl_X + maxLoc.x * pixelWidth, toMetr_Polygon_Tl_Y + maxLoc.y * pixelHeight);

            // смещение
            OGRFeature *poFeaturept_Swift;
            poFeaturept_Swift = OGRFeature::CreateFeature( poLayerPoint->GetLayerDefn() );
            poFeaturept_Swift->SetField(0, idCount );

            OGRPoint pt_Swift;
            pt_Swift.setX(shiftVectorEnd.x());
            pt_Swift.setY(shiftVectorEnd.y());

            // центр смещения
            OGRFeature *poFeaturept_Mid;
            poFeaturept_Mid = OGRFeature::CreateFeature( poLayerPoint->GetLayerDefn() );
            poFeaturept_Mid->SetField(0, idCount );

            OGRPoint pt_Mid;
            pt_Mid.setX(shiftVectorStart.x());
            pt_Mid.setY(shiftVectorStart.y());
            // линия смещения
            OGRFeature *poFeatureLine;
            poFeatureLine = OGRFeature::CreateFeature( poLayerLine->GetLayerDefn());
            poFeatureLine->SetField(0, idCount );
            OGRLineString line;

            line.addPoint(&pt_Swift);
            line.addPoint(&pt_Mid);

            // область анализа(линия)
            OGRFeature *poFeatureLine_Scope;
            poFeatureLine_Scope = OGRFeature::CreateFeature( poLayerLine_Scope->GetLayerDefn());
            poFeatureLine_Scope->SetField(0, idCount );

            OGRLineString line_Scope;
            OGRPoint pt_Tl;
            pt_Tl.setX(qScope.at(0).x());
            pt_Tl.setY(qScope.at(0).y());

            OGRPoint pt_Tr;
            pt_Tr.setX(qScope.at(1).x());
            pt_Tr.setY(qScope.at(1).y());

            OGRPoint pt_Br;
            pt_Br.setX(qScope.at(2).x());
            pt_Br.setY(qScope.at(2).y());

            OGRPoint pt_Bl;
            pt_Bl.setX(qScope.at(3).x());
            pt_Bl.setY(qScope.at(3).y());

            line_Scope.addPoint(&pt_Tl);
            line_Scope.addPoint(&pt_Tr);
            line_Scope.addPoint(&pt_Br);
            line_Scope.addPoint(&pt_Bl);
            line_Scope.addPoint(&pt_Tl);

            // полигон анализа
            OGRFeature *poFeature_Polygon_Area;
            poFeature_Polygon_Area = OGRFeature::CreateFeature( poLayerPolygon_Area->GetLayerDefn() );

            OGRPolygon polygon_Area;
            OGRLinearRing ogrring;

            ogrring.setPoint(0, &pt_Tl);
            ogrring.setPoint(1, &pt_Tr);
            ogrring.setPoint(2, &pt_Br);
            ogrring.setPoint(3, &pt_Bl);
            ogrring.setPoint(4, &pt_Tl);

            polygon_Area.addRing(&ogrring);

            poFeaturept_Mid->SetGeometry(&pt_Mid);
            poLayerPoint->CreateFeature(poFeaturept_Mid);
            OGRFeature::DestroyFeature(poFeaturept_Mid);

            poFeatureLine->SetGeometry(&line);
            poLayerLine->CreateFeature(poFeatureLine);
            OGRFeature::DestroyFeature(poFeatureLine);

            poFeatureLine_Scope->SetGeometry(&line_Scope);
            poLayerLine_Scope->CreateFeature(poFeatureLine_Scope);
            OGRFeature::DestroyFeature(poFeatureLine_Scope);

            poFeature_Polygon_Area->SetGeometry(&polygon_Area);
            poLayerPolygon_Area->CreateFeature(poFeature_Polygon_Area);
            OGRFeature::DestroyFeature(poFeature_Polygon_Area);

            // Release image's data
            delete [] (uchar*)refBuf;
            delete [] (uchar*)curBuf;

            ui->progressBar_calculateGeoShifts->setValue(ui->progressBar_calculateGeoShifts->value() + 1);
            QApplication::processEvents();
        }
    }

    QMessageBox::information(this, "Успех", "Изображение успешно обработано");
    ui->progressBar_calculateGeoShifts->setValue(0);
    ui->pB_calculateCommonArea->setEnabled(true);
    ui->pB_calculateGeoShifts->setEnabled(true);

    GDALClose((GDALDatasetH)poShapeDataSet);
}

void MainWindow::takeDFT(Mat& source, Mat& destination){

    Mat originalComplex[2] = {cv::Mat_<float>(source), Mat::zeros(source.size(), CV_32F)};

    Mat dftReady;

    cv::merge(originalComplex, 2, dftReady);

    Mat dftOriginal;

    dft(dftReady, dftOriginal, DFT_COMPLEX_OUTPUT);

    destination = dftOriginal;
}

void MainWindow::recenterDFT(Mat &source){

    int centerX = source.cols/2;
    int centerY = source.rows/2;

    Mat q1(source, Rect(0, 0, centerX, centerY));
    Mat q2(source, Rect(centerX, 0, centerX, centerY));
    Mat q3(source, Rect(0, centerY, centerX, centerY));
    Mat q4(source, Rect(centerX, centerY, centerX, centerY));

    Mat swapMap;

    q1.copyTo(swapMap);
    q4.copyTo(q1);
    swapMap.copyTo(q4);

    q2.copyTo(swapMap);
    q3.copyTo(q2);
    swapMap.copyTo(q3);
}

void MainWindow::compareSpectrums(Mat &refDFT, Mat &curDFT, double l, Point &maxLoc, double &maxVal, double &corrContrast, bool deleteAxis, bool invertCorrMat){

    // Обнулим левый верхний пиксель спектра первого изображения
    Mat reAndImOfDft1[] = {Mat(refDFT.rows, refDFT.cols, CV_32F), Mat(refDFT.rows, refDFT.cols, CV_32F)};
    split(refDFT, reAndImOfDft1);
    float* ptr1 = reAndImOfDft1[0].ptr<float>(0);
    ptr1[0] = 0.0;

    cv::merge(reAndImOfDft1, 2, refDFT);

    // обнулим левый верхний пиксель спектра второго изображения
    Mat reAndImOfDft2[] = {Mat(curDFT.rows, curDFT.cols, CV_32F), Mat(curDFT.rows, curDFT.cols, CV_32F)};
    split(curDFT, reAndImOfDft2);
    float* ptr2 = reAndImOfDft2[0].ptr<float>(0);
    ptr2[0] = 0.0;

    // Умножим спектры
    Mat multRe(reAndImOfDft1[0].rows, reAndImOfDft2[0].cols, CV_32F);
    Mat multIm(reAndImOfDft1[1].rows, reAndImOfDft2[1].cols, CV_32F);
    for (int row = 0;  row < reAndImOfDft1[0].rows; row++){
        float* ptrRe1 = reAndImOfDft1[0].ptr<float>(row);
        float* ptrIm1 = reAndImOfDft1[1].ptr<float>(row);
        float* ptrRe2 = reAndImOfDft2[0].ptr<float>(row);
        float* ptrIm2 = reAndImOfDft2[1].ptr<float>(row);

        float* ptrReOfMult = multRe.ptr<float>(row);
        float* ptrImOfMult = multIm.ptr<float>(row);

        for(int col = 0; col < reAndImOfDft1[0].cols; col++){
            float re1 = ptrRe1[col];
            float im1 = ptrIm1[col];
            float re2 = ptrRe2[col];
            float im2 = ptrIm2[col];

            float reofMult = re1 * re2 + im1 * im2;
            float imOfMult = re2 * im1 - re1 * im2;
            float absOfMult = sqrt(reofMult * reofMult + imOfMult * imOfMult) + 1;
            absOfMult = pow(absOfMult, 1 - l);
            reofMult = reofMult / absOfMult;
            imOfMult = imOfMult / absOfMult;
            ptrReOfMult[col] = reofMult;
            ptrImOfMult[col] = imOfMult;
        }
    }

    Mat multOfDft[] = {Mat(multRe), Mat(multIm)};
    Mat multOfDftReadyForInverse;
    cv::merge(multOfDft, 2, multOfDftReadyForInverse);

    // Обратное преобразование Фурье
    Mat invertedMulOfDft;
    dft(multOfDftReadyForInverse, invertedMulOfDft, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);

    recenterDFT(invertedMulOfDft);

    if (deleteAxis){
        // Обнулим осевые компоненты
        int x1 = invertedMulOfDft.cols / 2;
        int x2 = x1 - 1;
        int y1 = invertedMulOfDft.rows / 2;
        int y2 = y1 - 1;

        for (int row = 0 ; row < invertedMulOfDft.rows; row++){
            float* ptr = invertedMulOfDft.ptr<float>(row);
            ptr[x1] = 0;
            ptr[x2] = 0;
            if (row == y1 || row == y2){
                for (int col = 0; col < invertedMulOfDft.cols; col++){
                    ptr[col] = 0;
                }
            }
        }
    }

    if (invertCorrMat){
        for (int row = 0; row < invertedMulOfDft.rows; row++){
            float* ptr = invertedMulOfDft.ptr<float>(row);
            for (int col = 0; col < invertedMulOfDft.cols; col++){
                ptr[col] = std::abs(ptr[col]);
            }
        }
    }

    // Найдем максимум корреляционной матрицы
    double minVal;
    Point minLoc;
    minMaxLoc(invertedMulOfDft, &minVal, &maxVal, &minLoc, &maxLoc);

    // Вычислим корреляционный контраст
    int nrows = invertedMulOfDft.rows;
    int ncols = invertedMulOfDft.cols;

    float roXY = 0.0;
    float roMAX = maxVal;
    float kimCoef;
    float* ptrCorr;
    for (int i = 0; i < nrows; i++){
        ptrCorr = invertedMulOfDft.ptr<float>(i);
        for (int j = 0; j < ncols; j++){
            roXY += ptrCorr[j] * ptrCorr[j];
        }
    }
    roXY = sqrt(roXY / (ncols * nrows));
    kimCoef = roMAX / roXY;
    corrContrast = kimCoef;
}
