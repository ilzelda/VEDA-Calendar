#ifndef DAYWIDGET_H
#define DAYWIDGET_H

#include <QWidget>
#include <QDate>

namespace Ui {
class DayWidget;
}

class DayWidget : public QWidget
{
    Q_OBJECT

public:
    //explicit DayWidget(QWidget *parent = nullptr);
    explicit DayWidget(const QDate &date, QWidget *parent = nullptr);
    ~DayWidget();

signals:
    void clicked(const QDate &date); // 버튼처럼 작동하도록 신호 정의

private:
    Ui::DayWidget *ui;
    QDate date;
private slots:
    void handleButtonClick(); // 내부 버튼 클릭 처리

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // DAYWIDGET_H
