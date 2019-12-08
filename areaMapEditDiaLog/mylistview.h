#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H
#include <QListView>
#include <QMouseEvent>
#include <QPoint>


class MyListView : public QListView
{
    Q_OBJECT
public:
    MyListView(QWidget *parent = 0);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *e);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);
signals:
    void addItem(int i);
    void dropFinished();

private:
    void perforDrag();

    QPoint startPos;
};

#endif // MYLISTVIEW_H
