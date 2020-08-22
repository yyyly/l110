#ifndef RULEWIDGET_H
#define RULEWIDGET_H

#include <QDialog>
#include <QPaintEvent>
#include <QEvent>
#include <QFileDialog>
#include <QSound>
#include <QModelIndex>

#include"./csqltablemode.h"
#include"rulemodel.h"
#include"./customdata.h"
#include"soundplayer.h"
#include<QMap>
namespace Ui {
class RuleWidget;
}

class RuleWidget : public QDialog
{
    Q_OBJECT

public:
    explicit RuleWidget(QWidget *parent = 0);
    QMap<QString,Ruler> getRulers();
    ~RuleWidget();
protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
public slots:
    void tableClicked(const QModelIndex &index);
    void colorEditClicked();
    void playSound();
    void soundFileSelect();
    void applyButtonClicked();
    void cancelButtonClicked();
    void okButtonClicked();
    void closeButtonClicked();
private:
    QPoint mousePos;
    bool mouseIsPress;
    Ui::RuleWidget *ui;
    int lineWidth = 2;
    SoundPlayer *soundPlayer;
    QModelIndex currentIndex;
    QFileDialog *fielDialog;
    QString path;
    RuleModel model;
    QMap<QString,Ruler> rulers;
    QString mFilter;
signals:
    void rulerChange();

private slots:
    void on_checkBox_stateChanged(int arg1);
};

#endif // RULEWIDGET_H
