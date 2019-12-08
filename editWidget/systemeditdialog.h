#ifndef SYSTEMEDITDIALOG_H
#define SYSTEMEDITDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include"globaloption.h"


namespace Ui {
class SystemEditDialog;
}

class SystemEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SystemEditDialog(QWidget *parent = nullptr);
    QString getImageSaveDir();
    QString getVedioSaveDir();
    ~SystemEditDialog();

public slots:
    void setVedioSaveDir();
    void setImageSaveDir();
    void closeButtonClicked();
private:
    Ui::SystemEditDialog *ui;
    QPoint mousePos;
    bool mouseIsPress;
    QString defPicSaveDir = "";
    QString defVedioSaveDir = "";
protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    void showEvent(QShowEvent *event);
private slots:
    void on_dirSaveButton_clicked();
    void on_dirDefaultButton_clicked();

};

#endif // SYSTEMEDITDIALOG_H
