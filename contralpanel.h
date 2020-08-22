#ifndef CONTRALPANEL_H
#define CONTRALPANEL_H

#include <QWidget>
#include <QPushButton>
#include "customContro/cpushbutton.h"
#include "serchedit.h"

class ContralPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ContralPanel(QWidget *parent = nullptr);

signals:
    void defence();
    void disDefence();
    void setDevice();
    void serchTest(const QString str);
private slots:
    void on_defenceButton_clicked();
    void on_disDefenceButton_clicked();
    void on_setButton_clicked();
    void on_serch(const QString str);
private:
    QPushButton *defenceButton;
    QPushButton *disDefenceButton;
    CPushButton *setButton;
    SerchEdit *edit;
protected:
    void paintEvent(QPaintEvent *event);
public slots:
    void updateState(bool enble);
};

#endif // CONTRALPANEL_H
