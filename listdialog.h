#ifndef LISTDIALOG_H
#define LISTDIALOG_H


#include <QDate>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QAction>
#include <QString>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QDialog>

#include "Schedule.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ListDialog;
}
QT_END_NAMESPACE

class ListDialog : public QDialog
{
    Q_OBJECT

public:

    ListDialog(QWidget *parent = nullptr, const QDate &date = QDate::currentDate());
    //const QDate &date = QDate::currentDate() selected Date 받아오기 위해서
    
    ~ListDialog();

    QMap<QDate, QList<Schedule>>& sch_map;

    QWidget* createTodoItemWidget(
                                          const QString &title,
                                          const QString &location,
                                          const QDateTime &start,
                                          const QDateTime &end);
private:
    QDate selectedDate; // dayListAddClicked에서 dayWidgetClicked를 호출할때 selectedDate 필요해서추가

signals:
    void callDayWidgetClicked(const QDate &date);
private slots:
    void addListLine();

private:
    Ui::ListDialog *ui;
};
#endif // LISTDIALOG_H
