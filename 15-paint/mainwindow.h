#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // return functions
    string returnUserId();
    bool returnFillInfoButtonClicked();
    bool returnSaveInfoButtonClicked();
    bool returnLeftHanded();
    bool returnRightHanded();
    bool returnStrokedLeftArm();
    bool returnStrokedRightArm();
    bool returnStrokedBothArm();
    int returnMirroringStrength();
    int returnGloveIntensity();
    double returnGloveDeadzone();
    bool returnMirroringEnabled();
    string returnGameAttemptsFolder();
    void setGame(string g);
    string returnShapeToDraw();
    void resetShapeToDraw();

    //get functions
    void getTimeElapsed(double t);


private slots:

    void on_FillInfo_clicked();

    void on_SaveInfo_clicked();

    void on_LeftHanded_toggled(bool checked);

    void on_RightHanded_toggled(bool checked);

    void on_StrokedLeftArm_toggled(bool checked);

    void on_StrokedRightArm_toggled(bool checked);

    void on_StrokedBothArm_toggled(bool checked);

    void on_GloveIntensitySlider_sliderMoved(int position);

    void on_EnableMirroring_clicked();

    //7-25-25 JML toggle system update for 3-way ifs
    void on_mirrorCanvasToggle_clicked();
    void on_feedbackCanvasToggle_clicked();
    void on_canvasToggle_clicked();

    void on_SetMirroringStrength_clicked();

    void on_SetGloveDeadzone_clicked();

    void on_DrawShape1_clicked();

    void on_DrawShape2_clicked();

    void on_DrawShape3_clicked();

    void on_DrawShape4_clicked();

    void on_ClearCanvas_clicked();
private:
    Ui::MainWindow *ui;
    bool fillInfoButtonClicked = false;
    bool saveInfoButtonClicked = false;
    bool leftHanded = false;
    bool rightHanded = false;
    bool strokedLeftArm = false;
    bool strokedRightArm = false;
    bool strokedBothArm = false;
    int mirroringStrength = 2000;
    int gloveIntensity = 50;
    double gloveDeadzone = 0.005; //arbitrary values
    bool mirroringEnabled = false;
    bool mirrorCanvasToggle = false; //7-25-25 hijack for 3 way toggles jml
    bool feedbackCanvasToggle = false;
    bool canvasToggle = false;
    double timeElapsed = 0.0;


    string game = "magnets"; //default game is magnets
    string infoTxt = "/MagnetsInfo.txt";
    string gameAttemptsFolder = "/MagnetsAttempts";
    string shapeToDraw;

};

#endif // MAINWINDOW_H
