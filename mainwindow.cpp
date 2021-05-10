#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fieldViev = new FieldView(this);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->scrollArea->setWidget(fieldViev);

    buttonFindWayActive = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonEditField_clicked()
{
    buttonFindWayActive = false;
    ui->pushButtonFindWay->setStyleSheet("");
    fieldViev->OpenEditFieldWindow();
}

void MainWindow::on_pushButtonStartEnd_clicked()
{
    fieldViev->SetStartEndPoint();
}

void MainWindow::on_pushButtonWall_clicked()
{
    fieldViev->AddDeleteWall();
}

void MainWindow::on_pushButtonRandom_clicked()
{
    buttonFindWayActive = false;
    ui->pushButtonFindWay->setStyleSheet("");
    fieldViev->GenerateRandomField();
}

void MainWindow::on_pushButtonClearField_clicked()
{
    buttonFindWayActive = false;
    ui->pushButtonFindWay->setStyleSheet("");
    fieldViev->ClearField();
}

void MainWindow::on_pushButtonFindWay_clicked()
{
    if (buttonFindWayActive == true)
    {
        buttonFindWayActive = false;
        ui->pushButtonFindWay->setStyleSheet("");
    }
    else
    {
        buttonFindWayActive = true;
        ui->pushButtonFindWay->setStyleSheet("background-color: lightgreen;");
    }
    fieldViev->FindWay(buttonFindWayActive);
}

void MainWindow::on_pushButtonExit_clicked()
{
    this->close();
}
