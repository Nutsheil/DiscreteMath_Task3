#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPushButton>

#include "fieldview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void RandomField();
    void DrawField(QPainter& painter);
    void InitField();
    void ReplaceButtons();

private slots:
    void on_pushButtonEditField_clicked();
    void on_pushButtonStartEnd_clicked();
    void on_pushButtonWall_clicked();
    void on_pushButtonRandom_clicked();
    void on_pushButtonClearField_clicked();
    void on_pushButtonFindWay_clicked();
    void on_pushButtonExit_clicked();

private:
    Ui::MainWindow *ui;
    FieldView* fieldViev;
    bool buttonFindWayActive;
};
#endif // MAINWINDOW_H
