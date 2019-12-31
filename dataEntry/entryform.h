#ifndef ENTRYFORM_H
#define ENTRYFORM_H

#include <QDialog>
#include <QList>
#include <QDataWidgetMapper>
#include <QSqlTableModel>


enum
{
    NUM = 0,
    NAME = 1,
    PERSON_1_NAME = 2,
    PERSON_1_PHONE = 3,
    PERSON_2_NAME = 4,
    PERSON_2_PHONE = 5,
    ALARM_TYPE = 6,
    ON_SCENCE = 7,
    ALARM_TYPE_DES = 8
};

enum FORM_TYPE
{
    PART_FORM,
    ALARM_FORM
};

namespace Ui {
class EntryForm;
}

class EntryForm : public QDialog
{
    Q_OBJECT

public:
    explicit EntryForm(QWidget *parent = nullptr,QSqlTableModel *m = nullptr,int n = -1,FORM_TYPE t = PART_FORM);
    void initIdList();
    ~EntryForm();
private slots:
    void on_numLineEdit_editingFinished();

    void on_pushButton_clicked();

private:
    int num;
    Ui::EntryForm *ui;
    QSqlTableModel *modle;
    QDataWidgetMapper *mapper;
    QList<int> idList;
    FORM_TYPE type;
};

#endif // ENTRYFORM_H
