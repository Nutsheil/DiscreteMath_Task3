#include "fieldview.h"
#include "ui_fieldview.h"

#include <QMouseEvent>
#include <QPainter>
#include <QRandomGenerator>
#include <QDebug>

FieldView::FieldView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FieldView)
{
    ui->setupUi(this);

    fieldWidth = DEFAULT_FIELD_WIDTH;
    fieldHeight = DEFAULT_FIELD_HEIGHT;
    cellSize = DEFAULT_CEIL_SIZE;

    state = Action::Other;
    buttonFindWatActive = false;

    InitField();
}

FieldView::~FieldView()
{
    delete ui;
}

void FieldView::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    DrawField(painter);
    DrawWay(painter);
    painter.end();
}

void FieldView::mousePressEvent(QMouseEvent *event)
{
    size_t x = event->x() / cellSize;
    size_t y = event->y() / cellSize;
    if (x < fieldWidth && y < fieldHeight)
    {
        if (state == Action::StartEndPoint)
        {
            if (event->button() == Qt::LeftButton)
                if (field.at(y).at(x) == false)
                    startPoint = QPoint(x, y);

            if (event->button() == Qt::RightButton)
                if (field.at(y).at(x) == false)
                    endPoint = QPoint(x, y);

            FindWay(buttonFindWatActive);
            repaint();
        }
    }
}

void FieldView::mouseMoveEvent(QMouseEvent *event)
{
    size_t x = event->x() / cellSize;
    size_t y = event->y() / cellSize;
    if (x < fieldWidth && y < fieldHeight)
    {
        if (state == Action::EditWall)
        {
            if (event->buttons() == Qt::LeftButton)
                if (field.at(y).at(x) == false)
                    if (!(startPoint.x() == (int)x && startPoint.y() == (int)y) && !(endPoint.x() == (int)x && endPoint.y() == (int)y))
                        field.at(y).at(x) = true;

            if (event->buttons() == Qt::RightButton)
                if (field.at(y).at(x) == true)
                    field.at(y).at(x) = false;

            FindWay(buttonFindWatActive);
            repaint();
        }
    }
}

void FieldView::NewFieldSettings(size_t fieldwidth, size_t fieldheight, size_t cellsize)
{
    fieldWidth = fieldwidth;
    fieldHeight = fieldheight;
    cellSize = cellsize;
    state = Action::Other;
    field.clear();
    InitField();
}


void FieldView::OpenEditFieldWindow()
{
    state = Action::Other;
    buttonFindWatActive = false;
    way.clear();
    EditField* editFieldWindow = new EditField(fieldWidth, fieldHeight, cellSize, this);
    connect(editFieldWindow, SIGNAL(ApplyNewField(size_t, size_t, size_t)),
            this, SLOT(NewFieldSettings(size_t, size_t, size_t)));
    editFieldWindow->show();
}

void FieldView::SetStartEndPoint()
{
    state = Action::StartEndPoint;
}

void FieldView::AddDeleteWall()
{
    state = Action::EditWall;
}

void FieldView::GenerateRandomField()
{
    buttonFindWatActive = false;
    state = Action::Other;
    RandomField();
    startPoint = QPoint(-1, -1);
    endPoint = QPoint(-1, -1);
    way.clear();
    repaint();
}

void FieldView::ClearField()
{
    buttonFindWatActive = false;
    state = Action::Other;
    field.clear();
    InitField();
    startPoint = QPoint(-1, -1);
    endPoint = QPoint(-1, -1);
    repaint();
}

void FieldView::InitField()
{
    this->setMinimumHeight(fieldHeight*cellSize);
    this->setMinimumWidth(fieldWidth*cellSize);
    for (size_t i = 0; i != fieldHeight; ++i)
    {
        std::vector<bool> vec;
        field.push_back(vec);
        for (size_t j = 0; j != fieldWidth; ++j)
            field.at(i).push_back(false);
    }

    startPoint = QPoint(-1, -1);
    endPoint = QPoint(-1, -1);

    way.clear();
}

void FieldView::RandomField()
{
    for (size_t i = 0; i != fieldHeight; ++i)
    {
        for (size_t j = 0; j != fieldWidth; ++j)
        {
            if (QRandomGenerator::global()->bounded(5))
                field.at(i).at(j) = false;
            else
                field.at(i).at(j) = true;
        }
    }
}

void FieldView::DrawField(QPainter& painter)
{
    QBrush blackBrush(Qt::black);
    QBrush whiteBrush(Qt::white);
    for (size_t i = 0; i != fieldHeight; ++i)
    {
        for (size_t j = 0; j != fieldWidth; ++j)
        {
            QRect rect(j*cellSize, i*cellSize, cellSize, cellSize);
            if (field.at(i).at(j))
                painter.setBrush(blackBrush);
            else
                painter.setBrush(whiteBrush);
            painter.drawRect(rect);
        }
    }

    QPoint point(-1, -1);
    if (startPoint != point)
    {
        QBrush startBrush(Qt::green);
        QRect startRect(startPoint.x() * cellSize, startPoint.y() * cellSize, cellSize, cellSize);
        painter.setBrush(startBrush);
        painter.drawRect(startRect);
    }
    if (endPoint != point)
    {
        QBrush endBrush(Qt::red);
        QRect endRect(endPoint.x() * cellSize, endPoint.y() * cellSize, cellSize, cellSize);
        painter.setBrush(endBrush);
        painter.drawRect(endRect);
    }
}

void FieldView::DrawWay(QPainter &painter)
{
    if (way.empty())
        return;

    QBrush yellowBrush(Qt::yellow);
    painter.setBrush(yellowBrush);
    for (auto point : way)
    {
        QRect rect(point.x() * cellSize, point.y() * cellSize, cellSize, cellSize);
        painter.drawRect(rect);
    }
}

void FieldView::FindWay(bool switcher)
{
    if (switcher == true)
        buttonFindWatActive = true;
    else
    {
        buttonFindWatActive = false;
        way.clear();
        repaint();
        return;
    }

    if (startPoint == QPoint(-1, -1) || endPoint == QPoint(-1, -1))
        return;

    GenerateSteps();

    way.clear();

    if (steps.at(endPoint.y()).at(endPoint.x()) == 0)
        return;

    QPoint currentPosition = endPoint;
    int currentStep = steps.at(currentPosition.y()).at(currentPosition.x());

    while (currentPosition != startPoint)
    {
        int x = currentPosition.x();
        int y = currentPosition.y();

        if ((y + 1) < (int)fieldHeight && steps.at(y+1).at(x) != 0 && steps.at(y+1).at(x) < currentStep)
            currentPosition += QPoint(0, 1);
        else if ((y - 1) >= 0 && steps.at(y-1).at(x) != 0 && steps.at(y-1).at(x) < currentStep)
            currentPosition += QPoint(0, -1);
        else if ((x + 1) < (int)fieldWidth && steps.at(y).at(x+1) != 0 && steps.at(y).at(x+1) < currentStep)
            currentPosition += QPoint(1, 0);
        else if ((x - 1) >= 0 && steps.at(y).at(x-1) != 0 && steps.at(y).at(x-1) < currentStep)
            currentPosition += QPoint(-1, 0);

        if (currentPosition != startPoint)
            way.push_back(currentPosition);
        currentStep -= 1;
    }

    repaint();
}

void FieldView::GenerateSteps()
{
    steps.clear();
    for (size_t i = 0; i != fieldHeight; ++i)
    {
        std::vector<int> vec;
        steps.push_back(vec);
        for (size_t j = 0; j != fieldWidth; ++j)
            steps.at(i).push_back(0);
    }

    steps.at(startPoint.y()).at(startPoint.x()) = 1;

    list.push_back(startPoint);

    while (!list.empty())
    {
        QPoint currentPosition = list.front();
        list.pop_front();

        int x = currentPosition.x();
        int y = currentPosition.y();

        if ((y + 1) < (int)fieldHeight && !steps.at(y+1).at(x) && field.at(y+1).at(x) == false)
        {
            steps.at(y+1).at(x) = steps.at(y).at(x) + 1;
            list.push_back(QPoint(x, y + 1));
        }
        if ((y - 1) >= 0 && !steps.at(y-1).at(x) && field.at(y-1).at(x) == false)
        {
            steps.at(y-1).at(x) = steps.at(y).at(x) + 1;
            list.push_back(QPoint(x, y - 1));
        }
        if ((x + 1) < (int)fieldWidth && !steps.at(y).at(x+1) && field.at(y).at(x+1) == false)
        {
            steps.at(y).at(x+1) = steps.at(y).at(x) + 1;
            list.push_back(QPoint(x + 1, y));
        }
        if ((x - 1) >= 0 && !steps.at(y).at(x-1) && field.at(y).at(x-1) == false)
        {
            steps.at(y).at(x-1) = steps.at(y).at(x) + 1;
            list.push_back(QPoint(x - 1, y));
        }
    }
}
