#ifndef ENTRYFORM_H
#define ENTRYFORM_H

#include <QDialog>
#include <QList>
#include <QDataWidgetMapper>
#include <QSqlTableModel>

namespace Ui {
class EntryForm;
}

class EntryForm : public QDialog
{
    Q_OBJECT
public:
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
        ALARM_TYPE_DES = 8,
        ALARM_FROM_PART = 9
    };

    enum FORM_TYPE
    {
        PART_FORM,
        ALARM_FORM
    };

    explicit EntryForm(QWidget *parent = nullptr,QSqlTableModel *m = nullptr,int n = -1,FORM_TYPE t = PART_FORM);
    ~EntryForm();

private slots:
    void on_numLineEdit_editingFinished();
    void on_pushButton_clicked();
private:
    Ui::EntryForm *ui;
    QSqlTableModel *modle;
    QDataWidgetMapper *mapper;
    //QList<int> idList;
    int num;
    FORM_TYPE type;

    static QList<int> alarmTypeList;
};

#endif // ENTRYFORM_H
