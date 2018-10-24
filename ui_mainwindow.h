/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <glwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_6;
    QTabWidget *tabWidget;
    QWidget *tab_mainWindow;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pB_openCacheFolder;
    QLineEdit *le_cachePath;
    QHBoxLayout *horizontalLayout;
    QPushButton *pB_openRasterData;
    QLineEdit *le_rasterDataPath;
    QSpacerItem *verticalSpacer;
    QProgressBar *progressBar_detectClouds;
    QGroupBox *gB_totalProcessing;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_9;
    QLabel *lbl_totalProcessing_subRoiSize;
    QSpinBox *sB_totalProcessing_subRoiSize;
    QCheckBox *cB_useRecommendedSubRoiSize;
    QSpacerItem *horizontalSpacer_21;
    QHBoxLayout *horizontalLayout_18;
    QLabel *lbl_minBrightness;
    QSpinBox *sB_minBrightness;
    QHBoxLayout *horizontalLayout_14;
    QLabel *lbl_analyzingWindowSize;
    QSpinBox *sB_analyzingWindowSize;
    QHBoxLayout *horizontalLayout_19;
    QLabel *lbl_maxBrightness;
    QSpinBox *sB_maxBrightness;
    QHBoxLayout *horizontalLayout_15;
    QCheckBox *cB_useGammaCorrection;
    QDoubleSpinBox *sB_gammaCorrectionValue;
    QSpacerItem *horizontalSpacer_22;
    QHBoxLayout *horizontalLayout_17;
    QLabel *lbl_laplaceThreshold;
    QDoubleSpinBox *sB_laplaceThreshold;
    QSpacerItem *horizontalSpacer_24;
    QHBoxLayout *horizontalLayout_16;
    QPushButton *pB_detectCloudsInAllTiles;
    QSpacerItem *horizontalSpacer_23;
    QWidget *tab_visualization;
    QGridLayout *gridLayout_5;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *gB_viewBox;
    QGridLayout *gridLayout;
    QCheckBox *cB_showClouds;
    QPushButton *pB_cropRoi;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lbl_zoom;
    QLineEdit *le_zoom;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_10;
    GLWidget *openGLWidget;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *gB_parameters;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *lbl_subRoiSize;
    QComboBox *cB_subRoiSize;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *pB_initViewer;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_13;
    QLabel *lbl_viewBoxSize;
    QComboBox *cB_tileSize;
    QSpacerItem *horizontalSpacer_19;
    QSpacerItem *horizontalSpacer_8;
    QWidget *tab_calculateGeoShifts;
    QGridLayout *gridLayout_9;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *pB_downloadCurSample;
    QLineEdit *le_curSamplePath;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_11;
    QPushButton *pB_downloadRefSample;
    QLineEdit *le_refSamplePath;
    QHBoxLayout *horizontalLayout_12;
    QSpacerItem *horizontalSpacer_18;
    QPushButton *pB_calculateCommonArea;
    QSpacerItem *horizontalSpacer_17;
    QGroupBox *gB_curSampleOptions;
    QGridLayout *gridLayout_8;
    QLineEdit *le_curSampleWidth;
    QLabel *lbl_curSampleNChannels;
    QLabel *lbl_curSampleWidth;
    QLabel *lbl_curSampleHeight;
    QLineEdit *le_curSampleHeight;
    QLineEdit *le_curSampleNChannels;
    QSpacerItem *horizontalSpacer_14;
    QSpacerItem *horizontalSpacer_15;
    QSpacerItem *horizontalSpacer_16;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_20;
    QPushButton *pB_calculateGeoShifts;
    QLabel *lbl_calculateGeoShifts_analyzingWindowSize;
    QSpinBox *sB_calculateGeoShifts_analyzingWindowSize;
    QSpacerItem *horizontalSpacer_11;
    QGroupBox *gB_refSampleOptions;
    QGridLayout *gridLayout_7;
    QLineEdit *le_refSampleWidth;
    QLabel *lbl_refSampleNChannels;
    QLabel *lbl_refSampleWidth;
    QLineEdit *le_refSampleNChannels;
    QLineEdit *le_refSampleHeight;
    QLabel *lbl_refSampleHeight;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *horizontalSpacer_12;
    QSpacerItem *horizontalSpacer_13;
    QProgressBar *progressBar_calculateGeoShifts;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(738, 695);
        MainWindow->setAnimated(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setEnabled(true);
        gridLayout_6 = new QGridLayout(centralWidget);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_mainWindow = new QWidget();
        tab_mainWindow->setObjectName(QStringLiteral("tab_mainWindow"));
        gridLayout_4 = new QGridLayout(tab_mainWindow);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pB_openCacheFolder = new QPushButton(tab_mainWindow);
        pB_openCacheFolder->setObjectName(QStringLiteral("pB_openCacheFolder"));

        horizontalLayout_2->addWidget(pB_openCacheFolder);

        le_cachePath = new QLineEdit(tab_mainWindow);
        le_cachePath->setObjectName(QStringLiteral("le_cachePath"));
        le_cachePath->setReadOnly(true);

        horizontalLayout_2->addWidget(le_cachePath);


        gridLayout_4->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pB_openRasterData = new QPushButton(tab_mainWindow);
        pB_openRasterData->setObjectName(QStringLiteral("pB_openRasterData"));

        horizontalLayout->addWidget(pB_openRasterData);

        le_rasterDataPath = new QLineEdit(tab_mainWindow);
        le_rasterDataPath->setObjectName(QStringLiteral("le_rasterDataPath"));
        le_rasterDataPath->setReadOnly(true);

        horizontalLayout->addWidget(le_rasterDataPath);


        gridLayout_4->addLayout(horizontalLayout, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer, 4, 0, 1, 1);

        progressBar_detectClouds = new QProgressBar(tab_mainWindow);
        progressBar_detectClouds->setObjectName(QStringLiteral("progressBar_detectClouds"));
        progressBar_detectClouds->setValue(0);

        gridLayout_4->addWidget(progressBar_detectClouds, 3, 0, 1, 1);

        gB_totalProcessing = new QGroupBox(tab_mainWindow);
        gB_totalProcessing->setObjectName(QStringLiteral("gB_totalProcessing"));
        gridLayout_3 = new QGridLayout(gB_totalProcessing);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        lbl_totalProcessing_subRoiSize = new QLabel(gB_totalProcessing);
        lbl_totalProcessing_subRoiSize->setObjectName(QStringLiteral("lbl_totalProcessing_subRoiSize"));

        horizontalLayout_9->addWidget(lbl_totalProcessing_subRoiSize);

        sB_totalProcessing_subRoiSize = new QSpinBox(gB_totalProcessing);
        sB_totalProcessing_subRoiSize->setObjectName(QStringLiteral("sB_totalProcessing_subRoiSize"));
        sB_totalProcessing_subRoiSize->setMinimum(1);
        sB_totalProcessing_subRoiSize->setMaximum(512);

        horizontalLayout_9->addWidget(sB_totalProcessing_subRoiSize);

        cB_useRecommendedSubRoiSize = new QCheckBox(gB_totalProcessing);
        cB_useRecommendedSubRoiSize->setObjectName(QStringLiteral("cB_useRecommendedSubRoiSize"));

        horizontalLayout_9->addWidget(cB_useRecommendedSubRoiSize);


        gridLayout_3->addLayout(horizontalLayout_9, 0, 0, 1, 1);

        horizontalSpacer_21 = new QSpacerItem(11, 148, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_21, 0, 1, 5, 1);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        lbl_minBrightness = new QLabel(gB_totalProcessing);
        lbl_minBrightness->setObjectName(QStringLiteral("lbl_minBrightness"));

        horizontalLayout_18->addWidget(lbl_minBrightness);

        sB_minBrightness = new QSpinBox(gB_totalProcessing);
        sB_minBrightness->setObjectName(QStringLiteral("sB_minBrightness"));
        sB_minBrightness->setMaximum(255);
        sB_minBrightness->setValue(70);

        horizontalLayout_18->addWidget(sB_minBrightness);


        gridLayout_3->addLayout(horizontalLayout_18, 0, 2, 1, 1);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        lbl_analyzingWindowSize = new QLabel(gB_totalProcessing);
        lbl_analyzingWindowSize->setObjectName(QStringLiteral("lbl_analyzingWindowSize"));

        horizontalLayout_14->addWidget(lbl_analyzingWindowSize);

        sB_analyzingWindowSize = new QSpinBox(gB_totalProcessing);
        sB_analyzingWindowSize->setObjectName(QStringLiteral("sB_analyzingWindowSize"));
        sB_analyzingWindowSize->setMinimum(8);
        sB_analyzingWindowSize->setMaximum(256);

        horizontalLayout_14->addWidget(sB_analyzingWindowSize);


        gridLayout_3->addLayout(horizontalLayout_14, 1, 0, 1, 1);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        lbl_maxBrightness = new QLabel(gB_totalProcessing);
        lbl_maxBrightness->setObjectName(QStringLiteral("lbl_maxBrightness"));

        horizontalLayout_19->addWidget(lbl_maxBrightness);

        sB_maxBrightness = new QSpinBox(gB_totalProcessing);
        sB_maxBrightness->setObjectName(QStringLiteral("sB_maxBrightness"));
        sB_maxBrightness->setMaximum(255);
        sB_maxBrightness->setValue(128);

        horizontalLayout_19->addWidget(sB_maxBrightness);


        gridLayout_3->addLayout(horizontalLayout_19, 1, 2, 1, 1);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        cB_useGammaCorrection = new QCheckBox(gB_totalProcessing);
        cB_useGammaCorrection->setObjectName(QStringLiteral("cB_useGammaCorrection"));

        horizontalLayout_15->addWidget(cB_useGammaCorrection);

        sB_gammaCorrectionValue = new QDoubleSpinBox(gB_totalProcessing);
        sB_gammaCorrectionValue->setObjectName(QStringLiteral("sB_gammaCorrectionValue"));
        sB_gammaCorrectionValue->setMaximum(2);
        sB_gammaCorrectionValue->setSingleStep(0.1);
        sB_gammaCorrectionValue->setValue(0.4);

        horizontalLayout_15->addWidget(sB_gammaCorrectionValue);

        horizontalSpacer_22 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_22);


        gridLayout_3->addLayout(horizontalLayout_15, 2, 0, 1, 1);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        lbl_laplaceThreshold = new QLabel(gB_totalProcessing);
        lbl_laplaceThreshold->setObjectName(QStringLiteral("lbl_laplaceThreshold"));

        horizontalLayout_17->addWidget(lbl_laplaceThreshold);

        sB_laplaceThreshold = new QDoubleSpinBox(gB_totalProcessing);
        sB_laplaceThreshold->setObjectName(QStringLiteral("sB_laplaceThreshold"));
        sB_laplaceThreshold->setMaximum(1);
        sB_laplaceThreshold->setSingleStep(0.05);
        sB_laplaceThreshold->setValue(0.4);

        horizontalLayout_17->addWidget(sB_laplaceThreshold);

        horizontalSpacer_24 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_24);


        gridLayout_3->addLayout(horizontalLayout_17, 3, 0, 1, 1);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        pB_detectCloudsInAllTiles = new QPushButton(gB_totalProcessing);
        pB_detectCloudsInAllTiles->setObjectName(QStringLiteral("pB_detectCloudsInAllTiles"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        font.setStrikeOut(false);
        pB_detectCloudsInAllTiles->setFont(font);

        horizontalLayout_16->addWidget(pB_detectCloudsInAllTiles);

        horizontalSpacer_23 = new QSpacerItem(88, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_23);


        gridLayout_3->addLayout(horizontalLayout_16, 4, 0, 1, 1);


        gridLayout_4->addWidget(gB_totalProcessing, 2, 0, 1, 1);

        tabWidget->addTab(tab_mainWindow, QString());
        tab_visualization = new QWidget();
        tab_visualization->setObjectName(QStringLiteral("tab_visualization"));
        gridLayout_5 = new QGridLayout(tab_visualization);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer_4 = new QSpacerItem(188, 258, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);

        gB_viewBox = new QGroupBox(tab_visualization);
        gB_viewBox->setObjectName(QStringLiteral("gB_viewBox"));
        gridLayout = new QGridLayout(gB_viewBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        cB_showClouds = new QCheckBox(gB_viewBox);
        cB_showClouds->setObjectName(QStringLiteral("cB_showClouds"));

        gridLayout->addWidget(cB_showClouds, 1, 0, 1, 1);

        pB_cropRoi = new QPushButton(gB_viewBox);
        pB_cropRoi->setObjectName(QStringLiteral("pB_cropRoi"));

        gridLayout->addWidget(pB_cropRoi, 3, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lbl_zoom = new QLabel(gB_viewBox);
        lbl_zoom->setObjectName(QStringLiteral("lbl_zoom"));
        lbl_zoom->setMaximumSize(QSize(70, 20));

        horizontalLayout_3->addWidget(lbl_zoom);

        le_zoom = new QLineEdit(gB_viewBox);
        le_zoom->setObjectName(QStringLiteral("le_zoom"));
        le_zoom->setMaximumSize(QSize(50, 20));
        le_zoom->setReadOnly(true);

        horizontalLayout_3->addWidget(le_zoom);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout_3, 2, 0, 1, 1);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_10, 2, 1, 1, 1);

        openGLWidget = new GLWidget(gB_viewBox);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setMinimumSize(QSize(256, 256));
        openGLWidget->setMaximumSize(QSize(1024, 1024));
        openGLWidget->setSizeIncrement(QSize(256, 255));

        gridLayout->addWidget(openGLWidget, 0, 0, 1, 2);


        horizontalLayout_7->addWidget(gB_viewBox);

        horizontalSpacer_3 = new QSpacerItem(158, 258, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(168, 148, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        gB_parameters = new QGroupBox(tab_visualization);
        gB_parameters->setObjectName(QStringLiteral("gB_parameters"));
        gridLayout_2 = new QGridLayout(gB_parameters);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        lbl_subRoiSize = new QLabel(gB_parameters);
        lbl_subRoiSize->setObjectName(QStringLiteral("lbl_subRoiSize"));

        horizontalLayout_5->addWidget(lbl_subRoiSize);

        cB_subRoiSize = new QComboBox(gB_parameters);
        cB_subRoiSize->setObjectName(QStringLiteral("cB_subRoiSize"));

        horizontalLayout_5->addWidget(cB_subRoiSize);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        gridLayout_2->addLayout(horizontalLayout_5, 0, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);

        pB_initViewer = new QPushButton(gB_parameters);
        pB_initViewer->setObjectName(QStringLiteral("pB_initViewer"));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        pB_initViewer->setFont(font1);

        horizontalLayout_6->addWidget(pB_initViewer);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);


        gridLayout_2->addLayout(horizontalLayout_6, 2, 0, 1, 1);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        lbl_viewBoxSize = new QLabel(gB_parameters);
        lbl_viewBoxSize->setObjectName(QStringLiteral("lbl_viewBoxSize"));
        lbl_viewBoxSize->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_13->addWidget(lbl_viewBoxSize);

        cB_tileSize = new QComboBox(gB_parameters);
        cB_tileSize->setObjectName(QStringLiteral("cB_tileSize"));
        cB_tileSize->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_13->addWidget(cB_tileSize);

        horizontalSpacer_19 = new QSpacerItem(13, 80, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_19);


        gridLayout_2->addLayout(horizontalLayout_13, 1, 0, 1, 1);


        horizontalLayout_4->addWidget(gB_parameters);

        horizontalSpacer_8 = new QSpacerItem(178, 148, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);


        verticalLayout->addLayout(horizontalLayout_4);


        gridLayout_5->addLayout(verticalLayout, 0, 0, 1, 1);

        tabWidget->addTab(tab_visualization, QString());
        tab_calculateGeoShifts = new QWidget();
        tab_calculateGeoShifts->setObjectName(QStringLiteral("tab_calculateGeoShifts"));
        gridLayout_9 = new QGridLayout(tab_calculateGeoShifts);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        pB_downloadCurSample = new QPushButton(tab_calculateGeoShifts);
        pB_downloadCurSample->setObjectName(QStringLiteral("pB_downloadCurSample"));

        horizontalLayout_10->addWidget(pB_downloadCurSample);

        le_curSamplePath = new QLineEdit(tab_calculateGeoShifts);
        le_curSamplePath->setObjectName(QStringLiteral("le_curSamplePath"));

        horizontalLayout_10->addWidget(le_curSamplePath);


        gridLayout_9->addLayout(horizontalLayout_10, 1, 0, 1, 2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_2, 6, 0, 1, 2);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        pB_downloadRefSample = new QPushButton(tab_calculateGeoShifts);
        pB_downloadRefSample->setObjectName(QStringLiteral("pB_downloadRefSample"));

        horizontalLayout_11->addWidget(pB_downloadRefSample);

        le_refSamplePath = new QLineEdit(tab_calculateGeoShifts);
        le_refSamplePath->setObjectName(QStringLiteral("le_refSamplePath"));

        horizontalLayout_11->addWidget(le_refSamplePath);


        gridLayout_9->addLayout(horizontalLayout_11, 0, 0, 1, 2);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        horizontalSpacer_18 = new QSpacerItem(138, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_18);

        pB_calculateCommonArea = new QPushButton(tab_calculateGeoShifts);
        pB_calculateCommonArea->setObjectName(QStringLiteral("pB_calculateCommonArea"));
        pB_calculateCommonArea->setFont(font1);

        horizontalLayout_12->addWidget(pB_calculateCommonArea);

        horizontalSpacer_17 = new QSpacerItem(128, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_17);


        gridLayout_9->addLayout(horizontalLayout_12, 3, 0, 1, 2);

        gB_curSampleOptions = new QGroupBox(tab_calculateGeoShifts);
        gB_curSampleOptions->setObjectName(QStringLiteral("gB_curSampleOptions"));
        gridLayout_8 = new QGridLayout(gB_curSampleOptions);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        le_curSampleWidth = new QLineEdit(gB_curSampleOptions);
        le_curSampleWidth->setObjectName(QStringLiteral("le_curSampleWidth"));
        le_curSampleWidth->setFrame(false);
        le_curSampleWidth->setReadOnly(true);

        gridLayout_8->addWidget(le_curSampleWidth, 1, 2, 1, 1);

        lbl_curSampleNChannels = new QLabel(gB_curSampleOptions);
        lbl_curSampleNChannels->setObjectName(QStringLiteral("lbl_curSampleNChannels"));

        gridLayout_8->addWidget(lbl_curSampleNChannels, 0, 0, 1, 1);

        lbl_curSampleWidth = new QLabel(gB_curSampleOptions);
        lbl_curSampleWidth->setObjectName(QStringLiteral("lbl_curSampleWidth"));

        gridLayout_8->addWidget(lbl_curSampleWidth, 1, 0, 1, 1);

        lbl_curSampleHeight = new QLabel(gB_curSampleOptions);
        lbl_curSampleHeight->setObjectName(QStringLiteral("lbl_curSampleHeight"));

        gridLayout_8->addWidget(lbl_curSampleHeight, 2, 0, 1, 1);

        le_curSampleHeight = new QLineEdit(gB_curSampleOptions);
        le_curSampleHeight->setObjectName(QStringLiteral("le_curSampleHeight"));
        le_curSampleHeight->setFrame(false);
        le_curSampleHeight->setReadOnly(true);

        gridLayout_8->addWidget(le_curSampleHeight, 2, 2, 1, 1);

        le_curSampleNChannels = new QLineEdit(gB_curSampleOptions);
        le_curSampleNChannels->setObjectName(QStringLiteral("le_curSampleNChannels"));
        le_curSampleNChannels->setFrame(false);
        le_curSampleNChannels->setReadOnly(true);

        gridLayout_8->addWidget(le_curSampleNChannels, 0, 2, 1, 1);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_14, 0, 1, 1, 1);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_15, 1, 1, 1, 1);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_16, 2, 1, 1, 1);


        gridLayout_9->addWidget(gB_curSampleOptions, 2, 1, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalSpacer_20 = new QSpacerItem(148, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_20);

        pB_calculateGeoShifts = new QPushButton(tab_calculateGeoShifts);
        pB_calculateGeoShifts->setObjectName(QStringLiteral("pB_calculateGeoShifts"));

        horizontalLayout_8->addWidget(pB_calculateGeoShifts);

        lbl_calculateGeoShifts_analyzingWindowSize = new QLabel(tab_calculateGeoShifts);
        lbl_calculateGeoShifts_analyzingWindowSize->setObjectName(QStringLiteral("lbl_calculateGeoShifts_analyzingWindowSize"));

        horizontalLayout_8->addWidget(lbl_calculateGeoShifts_analyzingWindowSize);

        sB_calculateGeoShifts_analyzingWindowSize = new QSpinBox(tab_calculateGeoShifts);
        sB_calculateGeoShifts_analyzingWindowSize->setObjectName(QStringLiteral("sB_calculateGeoShifts_analyzingWindowSize"));
        sB_calculateGeoShifts_analyzingWindowSize->setMinimum(1);
        sB_calculateGeoShifts_analyzingWindowSize->setMaximum(2056);

        horizontalLayout_8->addWidget(sB_calculateGeoShifts_analyzingWindowSize);

        horizontalSpacer_11 = new QSpacerItem(148, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_11);


        gridLayout_9->addLayout(horizontalLayout_8, 4, 0, 1, 2);

        gB_refSampleOptions = new QGroupBox(tab_calculateGeoShifts);
        gB_refSampleOptions->setObjectName(QStringLiteral("gB_refSampleOptions"));
        gridLayout_7 = new QGridLayout(gB_refSampleOptions);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        le_refSampleWidth = new QLineEdit(gB_refSampleOptions);
        le_refSampleWidth->setObjectName(QStringLiteral("le_refSampleWidth"));
        le_refSampleWidth->setFrame(false);
        le_refSampleWidth->setReadOnly(true);

        gridLayout_7->addWidget(le_refSampleWidth, 1, 2, 1, 1);

        lbl_refSampleNChannels = new QLabel(gB_refSampleOptions);
        lbl_refSampleNChannels->setObjectName(QStringLiteral("lbl_refSampleNChannels"));

        gridLayout_7->addWidget(lbl_refSampleNChannels, 0, 0, 1, 1);

        lbl_refSampleWidth = new QLabel(gB_refSampleOptions);
        lbl_refSampleWidth->setObjectName(QStringLiteral("lbl_refSampleWidth"));

        gridLayout_7->addWidget(lbl_refSampleWidth, 1, 0, 1, 1);

        le_refSampleNChannels = new QLineEdit(gB_refSampleOptions);
        le_refSampleNChannels->setObjectName(QStringLiteral("le_refSampleNChannels"));
        le_refSampleNChannels->setFrame(false);
        le_refSampleNChannels->setReadOnly(true);

        gridLayout_7->addWidget(le_refSampleNChannels, 0, 2, 1, 1);

        le_refSampleHeight = new QLineEdit(gB_refSampleOptions);
        le_refSampleHeight->setObjectName(QStringLiteral("le_refSampleHeight"));
        le_refSampleHeight->setFrame(false);
        le_refSampleHeight->setReadOnly(true);

        gridLayout_7->addWidget(le_refSampleHeight, 2, 2, 1, 1);

        lbl_refSampleHeight = new QLabel(gB_refSampleOptions);
        lbl_refSampleHeight->setObjectName(QStringLiteral("lbl_refSampleHeight"));

        gridLayout_7->addWidget(lbl_refSampleHeight, 2, 0, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_9, 1, 1, 1, 1);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_12, 0, 1, 1, 1);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_13, 2, 1, 1, 1);


        gridLayout_9->addWidget(gB_refSampleOptions, 2, 0, 1, 1);

        progressBar_calculateGeoShifts = new QProgressBar(tab_calculateGeoShifts);
        progressBar_calculateGeoShifts->setObjectName(QStringLiteral("progressBar_calculateGeoShifts"));
        progressBar_calculateGeoShifts->setValue(0);

        gridLayout_9->addWidget(progressBar_calculateGeoShifts, 5, 0, 1, 2);

        tabWidget->addTab(tab_calculateGeoShifts, QString());

        gridLayout_6->addWidget(tabWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 738, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Big Raster Viewer (Zavarzin A.A.)", Q_NULLPTR));
        pB_openCacheFolder->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \321\202\320\260\320\271\320\273\320\276\320\262\321\213\320\271 \320\272\321\215\321\210", Q_NULLPTR));
        pB_openRasterData->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265", Q_NULLPTR));
        gB_totalProcessing->setTitle(QApplication::translate("MainWindow", "\320\237\320\276\321\202\320\276\320\272\320\276\320\262\320\260\321\217 \320\276\320\261\321\200\320\260\320\261\320\276\321\202\320\272\320\260", Q_NULLPTR));
        lbl_totalProcessing_subRoiSize->setText(QApplication::translate("MainWindow", "\320\240\320\260\320\267\320\274\320\265\321\200 \320\277\320\276\320\264\321\200\320\265\320\263\320\270\320\276\320\275\320\260: ", Q_NULLPTR));
        sB_totalProcessing_subRoiSize->setSuffix(QApplication::translate("MainWindow", " pix", Q_NULLPTR));
        cB_useRecommendedSubRoiSize->setText(QApplication::translate("MainWindow", "\320\230\321\201\320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\321\214 \321\200\320\265\320\272\320\276\320\274. \320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\265", Q_NULLPTR));
        lbl_minBrightness->setText(QApplication::translate("MainWindow", "\320\234\320\270\320\275. \321\217\321\200\320\272\320\276\321\201\321\202\321\214: ", Q_NULLPTR));
        lbl_analyzingWindowSize->setText(QApplication::translate("MainWindow", "\320\240\320\260\320\267\320\274\320\265\321\200 \320\260\320\275\320\260\320\273\320\270\320\267\320\270\321\200\321\203\321\216\321\211\320\265\320\263\320\276 \320\276\320\272\320\275\320\260 (\320\272\320\276\320\273-\320\262\320\276 \320\277\320\276\320\264\321\200\320\265\320\263\320\270\320\276\320\275\320\276\320\262 \320\277\320\276 \321\210\320\270\321\200\320\276\321\202\320\265/\320\262\321\213\321\201\320\276\321\202\320\265): ", Q_NULLPTR));
        lbl_maxBrightness->setText(QApplication::translate("MainWindow", "\320\234\320\260\320\272\321\201. \321\217\321\200\320\272\320\276\321\201\321\202\321\214: ", Q_NULLPTR));
        cB_useGammaCorrection->setText(QApplication::translate("MainWindow", "\320\222\321\213\320\277\320\276\320\273\320\275\320\270\321\202\321\214 \320\263\320\260\320\274\320\274\320\260-\320\272\320\276\321\200\321\200\320\265\320\272\321\206\320\270\321\216", Q_NULLPTR));
        lbl_laplaceThreshold->setText(QApplication::translate("MainWindow", "\320\236\320\277\320\265\321\200\320\260\321\202\320\276\321\200 \320\273\320\260\320\277\320\273\320\260\321\201\320\260 (\320\277\320\276\321\200\320\276\320\263. \320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\265): ", Q_NULLPTR));
        pB_detectCloudsInAllTiles->setText(QApplication::translate("MainWindow", "\320\237\320\276\321\202\320\276\320\272\320\276\320\262\320\260\321\217 \320\276\320\261\321\200\320\260\320\261\320\276\321\202\320\272\320\260", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_mainWindow), QApplication::translate("MainWindow", "\320\223\320\273\320\260\320\262\320\275\320\276\320\265 \320\276\320\272\320\275\320\276", Q_NULLPTR));
        gB_viewBox->setTitle(QString());
        cB_showClouds->setText(QApplication::translate("MainWindow", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214 \320\274\320\260\321\201\320\272\321\203 \320\276\320\261\320\273\320\260\321\207\320\275\320\276\321\201\321\202\320\270", Q_NULLPTR));
        pB_cropRoi->setText(QApplication::translate("MainWindow", "\320\222\321\213\321\200\320\265\320\267\320\260\321\202\321\214 \320\270 \321\201\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265", Q_NULLPTR));
        lbl_zoom->setText(QApplication::translate("MainWindow", "\320\243\320\262\320\265\320\273\320\270\321\207\320\265\320\275\320\270\320\265: ", Q_NULLPTR));
        gB_parameters->setTitle(QString());
        lbl_subRoiSize->setText(QApplication::translate("MainWindow", "\320\240\320\260\320\267\320\274\320\265\321\200 \320\277\320\276\320\264\321\200\320\265\320\263\320\270\320\276\320\275\320\260: ", Q_NULLPTR));
        pB_initViewer->setText(QApplication::translate("MainWindow", "\320\230\320\275\320\270\321\206\320\270\320\260\320\273\320\270\320\267\320\260\321\206\320\270\321\217", Q_NULLPTR));
        lbl_viewBoxSize->setText(QApplication::translate("MainWindow", "\320\240\320\260\320\267\320\274\320\265\321\200 \320\276\320\261\320\267\320\276\321\200\320\275\320\276\320\271 \321\200\320\260\320\274\320\272\320\270: ", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_visualization), QApplication::translate("MainWindow", "\320\222\320\270\320\267\321\203\320\260\320\273\320\270\320\267\320\260\321\206\320\270\321\217", Q_NULLPTR));
        pB_downloadCurSample->setText(QApplication::translate("MainWindow", "\320\242\320\265\320\272\321\203\321\211\320\265\320\265 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265: ", Q_NULLPTR));
        pB_downloadRefSample->setText(QApplication::translate("MainWindow", "\320\236\320\277\320\276\321\200\320\275\320\276\320\265 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265: ", Q_NULLPTR));
        pB_calculateCommonArea->setText(QApplication::translate("MainWindow", "\320\236\320\277\321\200\320\265\320\264\320\265\320\273\320\270\321\202\321\214 \320\276\320\261\321\211\321\203\321\216 \320\276\320\261\320\273\320\260\321\201\321\202\321\214", Q_NULLPTR));
        gB_curSampleOptions->setTitle(QApplication::translate("MainWindow", "\320\241\320\262\320\276\320\271\321\201\321\202\320\262\320\260 \321\202\320\265\320\272\321\203\321\211\320\265\320\263\320\276 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\321\217: ", Q_NULLPTR));
        lbl_curSampleNChannels->setText(QApplication::translate("MainWindow", "\320\232\320\276\320\273-\320\262\320\276 \320\272\320\260\320\275\320\260\320\273\320\276\320\262; ", Q_NULLPTR));
        lbl_curSampleWidth->setText(QApplication::translate("MainWindow", "\320\250\320\270\321\200\320\270\320\275\320\260: ", Q_NULLPTR));
        lbl_curSampleHeight->setText(QApplication::translate("MainWindow", "\320\222\321\213\321\201\320\276\321\202\320\260: ", Q_NULLPTR));
        le_curSampleNChannels->setText(QString());
        pB_calculateGeoShifts->setText(QApplication::translate("MainWindow", "\320\222\321\213\321\207\320\270\321\201\320\273\320\270\321\202\321\214 \321\201\320\274\320\265\321\211\320\265\320\275\320\270\321\217", Q_NULLPTR));
        lbl_calculateGeoShifts_analyzingWindowSize->setText(QApplication::translate("MainWindow", "\320\240\320\260\320\267\320\274\320\265\321\200 \320\260\320\275\320\260\320\273\320\270\320\267\320\270\321\200\321\203\320\265\320\274\320\276\320\263\320\276 \320\276\320\272\320\275\320\260: ", Q_NULLPTR));
        gB_refSampleOptions->setTitle(QApplication::translate("MainWindow", "\320\241\320\262\320\276\320\271\321\201\321\202\320\262\320\260 \320\276\320\277\320\276\321\200\320\275\320\276\320\263\320\276 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\321\217: ", Q_NULLPTR));
        lbl_refSampleNChannels->setText(QApplication::translate("MainWindow", "\320\232\320\276\320\273-\320\262\320\276 \320\272\320\260\320\275\320\260\320\273\320\276\320\262: ", Q_NULLPTR));
        lbl_refSampleWidth->setText(QApplication::translate("MainWindow", "\320\250\320\270\321\200\320\270\320\275\320\260: ", Q_NULLPTR));
        le_refSampleNChannels->setText(QString());
        lbl_refSampleHeight->setText(QApplication::translate("MainWindow", "\320\222\321\213\321\201\320\276\321\202\320\260: ", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_calculateGeoShifts), QApplication::translate("MainWindow", "\320\240\320\260\321\201\321\207\321\221\321\202 \321\201\320\274\320\265\321\211\320\265\320\275\320\270\321\217", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
