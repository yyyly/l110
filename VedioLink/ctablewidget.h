#ifndef CTABLEWIDGET_H
#define CTABLEWIDGET_H

#include <QDialog>

namespace Ui {
class CTableWidget;
}

class CTableWidget : public QDialog
{
    Q_OBJECT

public:
    explicit CTableWidget(QWidget *parent = nullptr);
    ~CTableWidget();

private:
    Ui::CTableWidget *ui;
};

#endif // CTABLEWIDGET_H
