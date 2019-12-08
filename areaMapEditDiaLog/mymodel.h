#ifndef MYMODEL_H
#define MYMODEL_H
#include<QAbstractListModel>

class MyModel : public QAbstractListModel
{
    Q_OBJECT
public:
    MyModel(const QList<int> &list,QObject *parent = Q_NULLPTR);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    QList<int> getBuffetList(){return bufferList;}
private:
    QList<int> list;
    QList<int> bufferList;
public slots:
    void remListVaule(int i);
    void addItem(int value);
};

#endif // MYMODEL_H
