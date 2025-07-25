#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include "chai3d.h"
#include <GLFW/glfw3.h>
using namespace std;
using namespace chai3d;

//COMMENTS FOR UPDATING GUI FILES
// to build, must run in terminal:
// moc -o moc_mainwindow.cpp mainwindow.h
// uic -o ui_mainwindow.h mainwindow.ui
//g++ -fPIC mainprogram.cpp mainwindow.cpp moc_mainwindow.cpp -o myapp `pkg-config --cflags --libs Qt5Widgets`


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //set paint game buttons invisible
    ui->LabelShapeToDraw->setVisible(false);
    ui->DrawShape1->setVisible(false);
    ui->DrawShape2->setVisible(false);
    ui->DrawShape3->setVisible(false);
    ui->DrawShape4->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//return functions
string MainWindow::returnUserId() {
    QString QID = ui->UserIdEntry->text();
    string ID = QID.toStdString();
    return ID;
}

bool MainWindow::returnFillInfoButtonClicked() {
    return fillInfoButtonClicked;
}

bool MainWindow::returnSaveInfoButtonClicked() {
    return saveInfoButtonClicked;
}

bool MainWindow::returnLeftHanded() {
    return leftHanded;
}

bool MainWindow::returnRightHanded() {
    return rightHanded;
}

bool MainWindow::returnStrokedLeftArm() {
    return strokedLeftArm;
}

bool MainWindow::returnStrokedRightArm() {
    return strokedRightArm;
}

bool MainWindow::returnStrokedBothArm() {
    return strokedBothArm;
}

int MainWindow::returnMirroringStrength() {
    return mirroringStrength;
}

int MainWindow::returnGloveIntensity() {
    return gloveIntensity;
}

double MainWindow::returnGloveDeadzone() {
    return gloveDeadzone;
}

bool MainWindow::returnMirroringEnabled() {
    return mirroringEnabled;
}

string MainWindow::returnGameAttemptsFolder() {
    return gameAttemptsFolder;
}

string MainWindow::returnShapeToDraw() {
    return shapeToDraw;
}
void MainWindow::resetShapeToDraw() {
    shapeToDraw = "";
}

//get functions
void MainWindow::getTimeElapsed(double t) {
    timeElapsed = t;
    string txt = "Elapsed Game Time: " + to_string(timeElapsed);
    QString Qtxt = QString::fromStdString(txt);
    ui->LabelGameTime->setText(Qtxt);
}

//set game function
void MainWindow::setGame(string g) {
    game = g;

    if (game == "magnets") {
        ui->LabelGame->setText("Magnets Game"); //set title
        infoTxt = "/MagnetsInfo.txt"; //set file locations and names
        gameAttemptsFolder = "/MagnetsAttempts";
    }

    if (game == "paint") {
        ui->LabelGame->setText("Paint Game");
        infoTxt = "/PaintInfo.txt"; //set file locations and names
        gameAttemptsFolder = "/PaintAttempts";
        ui->LabelShapeToDraw->setVisible(true);
        ui->DrawShape1->setVisible(true);
        ui->DrawShape2->setVisible(true);
        ui->DrawShape3->setVisible(true);
        ui->DrawShape4->setVisible(true);
    }
}

//slots functions

void MainWindow::on_FillInfo_clicked()
{
    //check if user exists
    if (returnUserId() != "") {
        auto currentpath = cGetCurrentPath();
        string folder = currentpath + "../Users/" + returnUserId();
        const char* folderPath = folder.c_str();
        bool user_exists;
        struct stat info;
        if (stat(folderPath, &info) != 0) {
            user_exists = false; //user does not exist
        }
        user_exists = (info.st_mode & S_IFDIR) != 0;

        if (!user_exists) {
            cout << "User does not exist" << endl;
            return; //if user does not exist, return
        }
        //get user file
        string userFilePath = currentpath + "../Users/" + returnUserId() + infoTxt;
        ifstream userFile(userFilePath);
        int i = 1; //line count
        //begin reading user file
        string dominantHand;
        string strokedArm;
        string line;
        while (getline(userFile, line)) {
            istringstream iss(line);
            string s;
            if (!(iss >> s)) {cout << "Error" << endl; break;} //error processing line

            if (i == 1) {
                dominantHand = s;
            }
            if (i == 2) {
                strokedArm = s;
            }
            if (i == 3) {
                mirroringStrength = stoi(s);
                QString sq = QString::fromStdString(s);
                ui->MirroringStrength->setText(sq);
            }
            if (i == 4) {
                gloveIntensity = stoi(s);
                ui->GloveIntensitySlider->setSliderPosition(stoi(s));
                ui->LabelGloveIntensityValue->setText(QString::fromStdString(s));
            }
            if (i == 5) {
                gloveDeadzone = stod(s);
                QString sq = QString::fromStdString(s);
                ui->GloveDeadzone->setText(sq);
            }
            i++;
        }

        //process dominant hand and stroked arm
        if (dominantHand == "LeftHand") {
            ui->LeftHanded->toggle();
        }
        else if (dominantHand == "RightHand") {
            ui->RightHanded->toggle();
        }

        if (strokedArm == "LeftArm") {
            ui->StrokedLeftArm->toggle();
        }
        else if (strokedArm == "RightArm") {
            ui->StrokedRightArm->toggle();
        }
        else if (strokedArm == "BothArms") {
            ui->StrokedBothArm->toggle();
        }


    fillInfoButtonClicked = true;
    }
}


void MainWindow::on_SaveInfo_clicked()
{
    if (returnUserId() != "") {
    //create necessary folders if they do not already exist
    auto currentpath = cGetCurrentPath();
    string folder = currentpath + "../Users";
    const char* folderPath = folder.c_str();
    mkdir(folderPath, 0777);
    folder = currentpath + "../Users/" + returnUserId();
    folderPath = folder.c_str();
    mkdir(folderPath, 0777);
    folder = currentpath + "../Users/" + returnUserId() + gameAttemptsFolder;
    folderPath = folder.c_str();
    mkdir(folderPath, 0777);

    //--output user information file--//
    //get dominant hand
    string dominantHand;
    if (leftHanded) {
        dominantHand = "LeftHand";
    }
    else if (rightHanded) {
        dominantHand = "RightHand";
    }
    else {
        dominantHand = "Neither";
    }

    //get stroked arm
    string strokedArm;
    if (strokedLeftArm) {
        strokedArm = "LeftArm";
    }
    else if (strokedRightArm) {
        strokedArm = "RightArm";
    }
    else if (strokedBothArm) {
        strokedArm = "BothArms";
    }

    time_t currentTime = time(0);
    tm* localCurrentTime = localtime(&currentTime);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%c", localCurrentTime);

    ofstream outputFile(currentpath + "../Users/" + returnUserId() + infoTxt, ios::out);

    if (outputFile.is_open())
    {
        outputFile << dominantHand << " :Dominant Hand\n" << strokedArm << " :Stroked Arm\n"
                   << mirroringStrength << " :Mirroring Strength\n" << gloveIntensity << " :Glove Intensity\n"
                   << gloveDeadzone << " :Glove Deadzone\n";
        outputFile.close();
    }
    else
    {
        cout << "Error opening file" << endl;
    }
    saveInfoButtonClicked = true;
    }
}


void MainWindow::on_LeftHanded_toggled(bool checked)
{
    leftHanded = checked;
    if (checked & ui->RightHanded->isChecked()) {
        ui->RightHanded->setChecked(false);
        rightHanded = false;
    }
}


void MainWindow::on_RightHanded_toggled(bool checked)
{
    rightHanded = checked;
    if (checked & ui->LeftHanded->isChecked()) {
        ui->LeftHanded->setChecked(false);
        leftHanded = false;
    }
}


void MainWindow::on_StrokedLeftArm_toggled(bool checked)
{
    strokedLeftArm = checked;
}


void MainWindow::on_StrokedRightArm_toggled(bool checked)
{
    strokedRightArm = checked;
}


void MainWindow::on_StrokedBothArm_toggled(bool checked)
{
    strokedBothArm = checked;
}


void MainWindow::on_GloveIntensitySlider_sliderMoved(int position)
{
    gloveIntensity = position;
    ui->LabelGloveIntensityValue->setText(QString::number(position));
}


void MainWindow::on_EnableMirroring_clicked()
{
    mirroringEnabled = true;
}

//7-10-25 JML mirrorCanvas toggling experiment
void MainWindow::on_mirrorCanvasToggle_clicked()
{
    //mirrorCanvasToggle = true;
    shapeToDraw ="drawingMirror";
}
//7-10-25 JML mirrorCanvas toggling experiment
void MainWindow::on_feedbackCanvasToggle_clicked()
{
    //mirrorCanvasToggle = true;
    shapeToDraw ="drawingFeedback";
}
//7-10-25 JML mirrorCanvas toggling experiment
void MainWindow::on_canvasToggle_clicked()
{
    //mirrorCanvasToggle = true;
    shapeToDraw ="drawingCanvas";
}

void MainWindow::on_SetMirroringStrength_clicked()
{
    mirroringStrength = ui->MirroringStrength->text().toInt();
}


void MainWindow::on_SetGloveDeadzone_clicked()
{
    gloveDeadzone = ui->GloveDeadzone->text().toDouble();
}


void MainWindow::on_DrawShape1_clicked()
{
    shapeToDraw = "drawing1";
}


void MainWindow::on_DrawShape2_clicked()
{
    shapeToDraw = "drawing2";
}


void MainWindow::on_DrawShape3_clicked()
{
    shapeToDraw = "drawing3";
}


void MainWindow::on_DrawShape4_clicked()
{
    shapeToDraw = "drawing4";
}

void MainWindow::on_ClearCanvas_clicked()
{
    shapeToDraw = "clearCanvas";
}

