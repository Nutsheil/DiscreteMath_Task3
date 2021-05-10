#ifndef FIELDVIEW_H
#define FIELDVIEW_H

#include <QWidget>

#include "editfield.h"

#include <QList>

#define DEFAULT_FIELD_WIDTH 40
#define DEFAULT_FIELD_HEIGHT 40
#define DEFAULT_CEIL_SIZE 20

enum Action{
    StartEndPoint,
    EditWall,
    Other
};

namespace Ui {
class FieldView;
}

class FieldView : public QWidget
{
    Q_OBJECT

public:
    explicit FieldView(QWidget *parent = nullptr);
    ~FieldView();
    void InitField();
    void RandomField();
    void DrawField(QPainter& painter);
    void DrawWay(QPainter& painter);
    void GenerateSteps();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void OpenEditFieldWindow();
    void SetStartEndPoint();
    void AddDeleteWall();
    void GenerateRandomField();
    void ClearField();
    void FindWay(bool switcher);

    void NewFieldSettings(size_t fieldwidth, size_t fieldheight, size_t cellsize);

private:
    Ui::FieldView *ui;

    size_t fieldWidth;
    size_t fieldHeight;
    size_t cellSize;
    std::vector<std::vector<bool>> field;
    std::vector<std::vector<int>> steps;
    QList<QPoint> list;
    QList<QPoint> way;
    QPoint startPoint;
    QPoint endPoint;
    Action state;
    bool buttonFindWatActive;
};

#endif // FIELDVIEW_H
