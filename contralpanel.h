#ifndef CONTRALPANEL_H
#define CONTRALPANEL_H

#include <QWidget>
#include <QPushButton>

class ContralPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ContralPanel(QWidget *parent = nullptr);

signals:
    void defence();
    void disDefence();
    void setDevice();

private slots:
    void on_defenceButton_clicked();
    void on_disDefenceButton_clicked();
    void on_setButton_clicked();
private:
    QPushButton *defenceButton;
    QPushButton *disDefenceButton;
    QPushButton *setButton;
public slots:
    void updateState(bool enble);
};

#endif // CONTRALPANEL_H
