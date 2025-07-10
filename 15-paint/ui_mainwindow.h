/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *LabelGame;
    QLabel *LabelUserId;
    QLineEdit *UserIdEntry;
    QRadioButton *LeftHanded;
    QRadioButton *RightHanded;
    QPushButton *FillInfo;
    QPushButton *SaveInfo;
    QLabel *LabelDominantHand;
    QLabel *LabelStrokedArm;
    QRadioButton *StrokedLeftArm;
    QRadioButton *StrokedRightArm;
    QRadioButton *StrokedBothArm;
    QLabel *LabelGameTime;
    QLabel *LabelMirroringStrength;
    QLineEdit *MirroringStrength;
    QPushButton *SetMirroringStrength;
    QPushButton *EnableMirroring;
    QLabel *LabelGloveIntensity;
    QSlider *GloveIntensitySlider;
    QLabel *LabelGloveIntensityValue;
    QLabel *LabelGloveDeadzone;
    QLineEdit *GloveDeadzone;
    QPushButton *SetGloveDeadzone;
    QLabel *LabelMirroringStrengthLimit;
    QPushButton *DrawShape1;
    QPushButton *DrawShape2;
    QPushButton *DrawShape3;
    QPushButton *DrawShape4;
    QLabel *LabelShapeToDraw;
    QPushButton *ClearCanvas;
    QPushButton *mirrorCanvasToggle;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        LabelGame = new QLabel(centralwidget);
        LabelGame->setObjectName(QString::fromUtf8("LabelGame"));
        LabelGame->setGeometry(QRect(320, 10, 181, 41));
        QFont font;
        font.setPointSize(18);
        LabelGame->setFont(font);
        LabelGame->setAlignment(Qt::AlignCenter);
        LabelUserId = new QLabel(centralwidget);
        LabelUserId->setObjectName(QString::fromUtf8("LabelUserId"));
        LabelUserId->setGeometry(QRect(60, 80, 191, 17));
        UserIdEntry = new QLineEdit(centralwidget);
        UserIdEntry->setObjectName(QString::fromUtf8("UserIdEntry"));
        UserIdEntry->setGeometry(QRect(260, 80, 113, 25));
        LeftHanded = new QRadioButton(centralwidget);
        LeftHanded->setObjectName(QString::fromUtf8("LeftHanded"));
        LeftHanded->setGeometry(QRect(250, 130, 112, 23));
        LeftHanded->setCheckable(true);
        LeftHanded->setChecked(false);
        LeftHanded->setAutoExclusive(false);
        RightHanded = new QRadioButton(centralwidget);
        RightHanded->setObjectName(QString::fromUtf8("RightHanded"));
        RightHanded->setGeometry(QRect(410, 130, 112, 23));
        RightHanded->setAutoExclusive(false);
        FillInfo = new QPushButton(centralwidget);
        FillInfo->setObjectName(QString::fromUtf8("FillInfo"));
        FillInfo->setGeometry(QRect(470, 80, 89, 25));
        SaveInfo = new QPushButton(centralwidget);
        SaveInfo->setObjectName(QString::fromUtf8("SaveInfo"));
        SaveInfo->setGeometry(QRect(590, 80, 89, 25));
        LabelDominantHand = new QLabel(centralwidget);
        LabelDominantHand->setObjectName(QString::fromUtf8("LabelDominantHand"));
        LabelDominantHand->setGeometry(QRect(90, 130, 111, 20));
        LabelStrokedArm = new QLabel(centralwidget);
        LabelStrokedArm->setObjectName(QString::fromUtf8("LabelStrokedArm"));
        LabelStrokedArm->setGeometry(QRect(100, 190, 91, 17));
        StrokedLeftArm = new QRadioButton(centralwidget);
        StrokedLeftArm->setObjectName(QString::fromUtf8("StrokedLeftArm"));
        StrokedLeftArm->setGeometry(QRect(240, 190, 112, 23));
        StrokedLeftArm->setAutoExclusive(true);
        StrokedRightArm = new QRadioButton(centralwidget);
        StrokedRightArm->setObjectName(QString::fromUtf8("StrokedRightArm"));
        StrokedRightArm->setGeometry(QRect(390, 190, 112, 23));
        StrokedRightArm->setAutoExclusive(true);
        StrokedBothArm = new QRadioButton(centralwidget);
        StrokedBothArm->setObjectName(QString::fromUtf8("StrokedBothArm"));
        StrokedBothArm->setGeometry(QRect(530, 190, 112, 23));
        StrokedBothArm->setAutoExclusive(true);
        LabelGameTime = new QLabel(centralwidget);
        LabelGameTime->setObjectName(QString::fromUtf8("LabelGameTime"));
        LabelGameTime->setGeometry(QRect(540, 20, 201, 17));
        LabelMirroringStrength = new QLabel(centralwidget);
        LabelMirroringStrength->setObjectName(QString::fromUtf8("LabelMirroringStrength"));
        LabelMirroringStrength->setGeometry(QRect(40, 360, 141, 20));
        LabelMirroringStrength->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        MirroringStrength = new QLineEdit(centralwidget);
        MirroringStrength->setObjectName(QString::fromUtf8("MirroringStrength"));
        MirroringStrength->setGeometry(QRect(190, 360, 113, 25));
        SetMirroringStrength = new QPushButton(centralwidget);
        SetMirroringStrength->setObjectName(QString::fromUtf8("SetMirroringStrength"));
        SetMirroringStrength->setGeometry(QRect(310, 360, 89, 25));
        EnableMirroring = new QPushButton(centralwidget);
        EnableMirroring->setObjectName(QString::fromUtf8("EnableMirroring"));
        EnableMirroring->setGeometry(QRect(190, 300, 131, 25));
        LabelGloveIntensity = new QLabel(centralwidget);
        LabelGloveIntensity->setObjectName(QString::fromUtf8("LabelGloveIntensity"));
        LabelGloveIntensity->setGeometry(QRect(70, 410, 111, 20));
        LabelGloveIntensity->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        GloveIntensitySlider = new QSlider(centralwidget);
        GloveIntensitySlider->setObjectName(QString::fromUtf8("GloveIntensitySlider"));
        GloveIntensitySlider->setGeometry(QRect(190, 410, 111, 21));
        GloveIntensitySlider->setCursor(QCursor(Qt::SplitHCursor));
        GloveIntensitySlider->setMaximum(20);
        GloveIntensitySlider->setSingleStep(1);
        GloveIntensitySlider->setPageStep(2);
        GloveIntensitySlider->setOrientation(Qt::Horizontal);
        GloveIntensitySlider->setTickPosition(QSlider::TicksBelow);
        GloveIntensitySlider->setTickInterval(2);
        LabelGloveIntensityValue = new QLabel(centralwidget);
        LabelGloveIntensityValue->setObjectName(QString::fromUtf8("LabelGloveIntensityValue"));
        LabelGloveIntensityValue->setGeometry(QRect(210, 430, 67, 17));
        LabelGloveIntensityValue->setAlignment(Qt::AlignCenter);
        LabelGloveDeadzone = new QLabel(centralwidget);
        LabelGloveDeadzone->setObjectName(QString::fromUtf8("LabelGloveDeadzone"));
        LabelGloveDeadzone->setGeometry(QRect(60, 460, 121, 20));
        LabelGloveDeadzone->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        GloveDeadzone = new QLineEdit(centralwidget);
        GloveDeadzone->setObjectName(QString::fromUtf8("GloveDeadzone"));
        GloveDeadzone->setGeometry(QRect(190, 460, 113, 25));
        SetGloveDeadzone = new QPushButton(centralwidget);
        SetGloveDeadzone->setObjectName(QString::fromUtf8("SetGloveDeadzone"));
        SetGloveDeadzone->setGeometry(QRect(310, 460, 89, 25));
        LabelMirroringStrengthLimit = new QLabel(centralwidget);
        LabelMirroringStrengthLimit->setObjectName(QString::fromUtf8("LabelMirroringStrengthLimit"));
        LabelMirroringStrengthLimit->setGeometry(QRect(190, 340, 111, 17));
        QFont font1;
        font1.setPointSize(8);
        LabelMirroringStrengthLimit->setFont(font1);
        LabelMirroringStrengthLimit->setAlignment(Qt::AlignCenter);
        DrawShape1 = new QPushButton(centralwidget);
        DrawShape1->setObjectName(QString::fromUtf8("DrawShape1"));
        DrawShape1->setGeometry(QRect(470, 350, 101, 41));
        DrawShape2 = new QPushButton(centralwidget);
        DrawShape2->setObjectName(QString::fromUtf8("DrawShape2"));
        DrawShape2->setGeometry(QRect(630, 350, 101, 41));
        DrawShape3 = new QPushButton(centralwidget);
        DrawShape3->setObjectName(QString::fromUtf8("DrawShape3"));
        DrawShape3->setGeometry(QRect(470, 460, 101, 41));
        DrawShape4 = new QPushButton(centralwidget);
        DrawShape4->setObjectName(QString::fromUtf8("DrawShape4"));
        DrawShape4->setGeometry(QRect(630, 460, 101, 41));
        LabelShapeToDraw = new QLabel(centralwidget);
        LabelShapeToDraw->setObjectName(QString::fromUtf8("LabelShapeToDraw"));
        LabelShapeToDraw->setGeometry(QRect(500, 310, 211, 21));
        QFont font2;
        font2.setPointSize(13);
        LabelShapeToDraw->setFont(font2);
        LabelShapeToDraw->setAlignment(Qt::AlignCenter);
        ClearCanvas = new QPushButton(centralwidget);
        ClearCanvas->setObjectName(QString::fromUtf8("ClearCanvas"));
        ClearCanvas->setGeometry(QRect(560, 520, 89, 25));
        mirrorCanvasToggle = new QPushButton(centralwidget);
        mirrorCanvasToggle->setObjectName(QString::fromUtf8("mirrorCanvasToggle"));
        mirrorCanvasToggle->setGeometry(QRect(200, 520, 151, 25));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        LabelGame->setText(QApplication::translate("MainWindow", "Magnets Game", nullptr));
        LabelUserId->setText(QApplication::translate("MainWindow", "Please Enter Your User ID:", nullptr));
        LeftHanded->setText(QApplication::translate("MainWindow", "Left Handed", nullptr));
        RightHanded->setText(QApplication::translate("MainWindow", "Right Handed", nullptr));
        FillInfo->setText(QApplication::translate("MainWindow", "Fill Info", nullptr));
        SaveInfo->setText(QApplication::translate("MainWindow", "Save Info", nullptr));
        LabelDominantHand->setText(QApplication::translate("MainWindow", "Dominant Hand:", nullptr));
        LabelStrokedArm->setText(QApplication::translate("MainWindow", "Stroked Arm:", nullptr));
        StrokedLeftArm->setText(QApplication::translate("MainWindow", "Left Arm", nullptr));
        StrokedRightArm->setText(QApplication::translate("MainWindow", "Right Arm", nullptr));
        StrokedBothArm->setText(QApplication::translate("MainWindow", "Both Arms", nullptr));
        LabelGameTime->setText(QApplication::translate("MainWindow", "Elapsed Game Time: ", nullptr));
        LabelMirroringStrength->setText(QApplication::translate("MainWindow", "Mirroring Strength: ", nullptr));
        MirroringStrength->setInputMask(QString());
        SetMirroringStrength->setText(QApplication::translate("MainWindow", "Set", nullptr));
        EnableMirroring->setText(QApplication::translate("MainWindow", "Enable Mirroring", nullptr));
        LabelGloveIntensity->setText(QApplication::translate("MainWindow", "Glove Intensity: ", nullptr));
        LabelGloveIntensityValue->setText(QApplication::translate("MainWindow", "Value", nullptr));
        LabelGloveDeadzone->setText(QApplication::translate("MainWindow", "Glove Deadzone:", nullptr));
        SetGloveDeadzone->setText(QApplication::translate("MainWindow", "Set", nullptr));
        LabelMirroringStrengthLimit->setText(QApplication::translate("MainWindow", "(No more than 3000)", nullptr));
        DrawShape1->setText(QApplication::translate("MainWindow", "Draw Circle", nullptr));
        DrawShape2->setText(QApplication::translate("MainWindow", "Draw Arrow \n"
"And Circle", nullptr));
        DrawShape3->setText(QApplication::translate("MainWindow", "Draw Triangle", nullptr));
        DrawShape4->setText(QApplication::translate("MainWindow", "Draw Square", nullptr));
        LabelShapeToDraw->setText(QApplication::translate("MainWindow", "Choose a shape to draw", nullptr));
        ClearCanvas->setText(QApplication::translate("MainWindow", "Clear Canvas", nullptr));
        mirrorCanvasToggle->setText(QApplication::translate("MainWindow", "Mirror Canvas Toggle", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
