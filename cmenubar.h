#ifndef CMENUBAR_H
#define CMENUBAR_H

#include <QWidget>
#include <QPushButton>
#include<QHBoxLayout>
#include<QTabBar>
#include<QLabel>
#include<QPushButton>
#include<QCloseEvent>
#include<QMenu>
#include<QMenuBar>

class CMenuBar : public QWidget
{
    Q_OBJECT
public:
    explicit CMenuBar(QWidget *parent = 0);
    QAction *addTitleAction(const QString &str);
    void setButtonStyle(QPushButton *button, QString imgsrc, int CutSec);
    void showNormal();
    void showMax();
private:
    QPushButton *closeButton;
    QPushButton *toMinButton;
    QPushButton *toMaxButton;
    QTabBar *tabBar;
    QMenuBar *Icon;
    QPushButton *accountButton;
    QLabel *nickNameLabel;
    QMenu* menu;
    QMenu *userMenu;
signals:
    void TableCliecked(int n);
    void toMaxSize();
    void toMinSize();
    void changeAccount();
    void manageAccount();
    void about();
public slots:
    void tableClicked(int n);
    void toMaxButtonClicked();
    void toMinButtonClicked();
    void accountButtonClicked();
    void updateNickName(const QString &name);
    void ChangeAccountSlot();
    void manageAccountSlot();
    void aboutSlot();
protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // CMENUBAR_H
