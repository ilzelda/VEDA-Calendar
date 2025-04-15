#ifndef LISTDIALOG_H
#define LISTDIALOG_H

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
QT_BEGIN_NAMESPACE
namespace Ui {
class ListDialog;
}
QT_END_NAMESPACE

class ListDialog : public QDialog
{
    Q_OBJECT

public:
    ListDialog(QWidget *parent = nullptr);
    ~ListDialog();

private:
    QWidget* createTodoItemWidget(QListWidgetItem* item);

private slots:
    void addListLine();
    void savefile();
    void loadfile();

private:
    Ui::ListDialog *ui;
};
#endif // LISTDIALOG_H
