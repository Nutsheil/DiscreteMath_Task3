#ifndef EDITFIELD_H
#define EDITFIELD_H

#include <QDialog>

namespace Ui {
class EditField;
}

class EditField : public QDialog
{
    Q_OBJECT

public:
    explicit EditField(size_t fieldwidth, size_t fieldheight, size_t cellsize, QWidget *parent = nullptr);
    ~EditField();

signals:
    void ApplyNewField(size_t fieldwidth, size_t fieldheight, size_t cellsize);

private slots:
    void on_pushButtonApply_clicked();

private:
    Ui::EditField *ui;

    size_t fieldWidth;
    size_t fieldHeight;
    size_t cellSize;
};

#endif // EDITFIELD_H
