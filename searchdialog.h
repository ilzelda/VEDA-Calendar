#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();

    QString target_title;

private:
    Ui::SearchDialog *ui;


private slots:
    void onSubmit();
};

#endif // SEARCHDIALOG_H
