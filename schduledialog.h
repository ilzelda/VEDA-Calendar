#ifndef SCHDULEDIALOG_H
#define SCHDULEDIALOG_H

#include <QWidget>

namespace Ui {
class SchduleDialog;
}

class SchduleDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SchduleDialog(QWidget *parent = nullptr);
    ~SchduleDialog();

private:
    Ui::SchduleDialog *ui;
};

#endif // SCHDULEDIALOG_H
