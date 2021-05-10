#include "editfield.h"
#include "ui_editfield.h"

EditField::EditField(size_t fieldwidth, size_t fieldheight, size_t cellsize, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditField),
    fieldWidth(fieldwidth),
    fieldHeight(fieldheight),
    cellSize(cellsize)
{
    ui->setupUi(this);

    ui->spinBoxWidth->setValue(fieldWidth);
    ui->spinBoxHeight->setValue(fieldHeight);
    ui->spinBoxCellSize->setValue(cellSize);
}

EditField::~EditField()
{
    delete ui;
}

void EditField::on_pushButtonApply_clicked()
{
    size_t newWidth = (size_t)ui->spinBoxWidth->value();
    size_t newHeight = (size_t)ui->spinBoxHeight->value();
    size_t newCellSize = (size_t)ui->spinBoxCellSize->value();
    if (fieldWidth == newWidth && fieldHeight == newHeight && cellSize == newCellSize)
    {
        this->close();
        return;
    }

    emit ApplyNewField(newWidth, newHeight, newCellSize);
    this->close();
}
