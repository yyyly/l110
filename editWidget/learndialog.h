#ifndef LEARNDIALOG_H
#define LEARNDIALOG_H

#include <QDialog>
#include "z1801.h"

namespace Ui {
class LearnDialog;
}

class LearnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LearnDialog(QWidget *parent = nullptr,Z1801 *z1801 = nullptr);
    ~LearnDialog();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_learnPushButton_clicked();

    void on_deltePushButton_clicked();

    void handelCodeMessing(CodeImfor imf);

private:
    Ui::LearnDialog *ui;
    Z1801 *d;
};

#endif // LEARNDIALOG_H
