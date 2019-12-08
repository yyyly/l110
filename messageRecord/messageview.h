#ifndef MESSAGEVIEW_H
#define MESSAGEVIEW_H

#include <QWidget>
#include <QTableView>

class MessageView : public QWidget
{
    Q_OBJECT
public:
    explicit MessageView(QWidget *parent = 0);

signals:

public slots:
public:
    QTableView *view;
};

#endif // MESSAGEVIEW_H
