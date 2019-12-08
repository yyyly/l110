#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <QWidget>
#include <QTreeView>
#include "preview/devicetreemode.h"
#include "preview/sqldata.h"

class PlayBack : public QWidget
{
    Q_OBJECT
public:
    explicit PlayBack(QWidget *parent = 0);

signals:

public slots:
private:
    QTreeView *treeView;
};

#endif // PLAYBACK_H
