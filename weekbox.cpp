#include "weekbox.h"
#include <QPainter>
#include <QtGlobal> // qMin 사용

#include "daywidget.h"

WeekBox::WeekBox(QDate _month, int n, QWidget *parent)
    : QGraphicsView(parent), nth_week(n), month(_month)
{
    scene = new QGraphicsScene(this);
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 프레임 제거 (원한다면)
    setFrameStyle(QFrame::NoFrame);

    // QGraphicsView 배경을 투명하게 설정
    setStyleSheet("background: transparent;");
    setAttribute(Qt::WA_TranslucentBackground);

    // Scene의 배경도 투명하게 처리
    scene->setBackgroundBrush(Qt::NoBrush);

    setAttribute(Qt::WA_TransparentForMouseEvents);

    min_day = n * 7 + 1;
    max_day = qMin(min_day + 6, month.daysInMonth());
}

WeekBox::~WeekBox()
{
}

void WeekBox::drawSchedules()
{
    bool DEBUG = true;
    // bool DEBUG = false;
    // if (DEBUG) qDebug()<<"[drawSchedules()] Rendering"<<nth_week<< "th week : ";

    scene->clear();
    // 위젯 전체 영역 (QGraphicsView의 viewport 크기를 사용)
    QRectF widgetRect(0, 0, scene->width() - 1, scene->height() - 1);

    // 디버그용 경계선을 추가 (원래 paintEvent에서 그리던 테두리)
    if (DEBUG)
    {
        scene->addRect(widgetRect, QPen(Qt::black), QBrush(Qt::NoBrush));
        // qDebug()<<"[drawSchedules()|DEBUG] 테두리";
    }


    // 가로 7등분: 각 칸의 너비 계산
    int cellWidth = widgetRect.width() / 7;

    // 세로 높이 결정:
    int scheduleCount = (schedule_list.size() > 0) ? schedule_list.size() : 1; // 스케줄 없으면 1
    int cellHeight = std::min(static_cast<int>(widgetRect.height() / scheduleCount),
                              static_cast<int>(widgetRect.height() / 2));

    // 각 스케줄마다 사각형과 텍스트 아이템 생성
    for (int i = 0; i < schedule_list.size(); i++) {
        const Schedule& sched = schedule_list.at(i);
        int start_day;
        int end_day;

        if (month.month() < sched.end.date().month())
        {
            start_day = std::max(sched.start.date().day(), min_day);
            end_day = max_day;
        }
        else if (sched.start.date().month() < month.month())
        {
            start_day = min_day;
            end_day = std::min(sched.end.date().day(), max_day);
        }
        else
        {
            start_day = std::max(sched.start.date().day(), min_day);
            end_day = std::min(sched.end.date().day(), max_day);
        }

        if (DEBUG) qDebug() << "start_day:" << start_day << "end_day:" << end_day;

        int start_x = (start_day - 1) % 7;
        int end_x = (end_day - 1) % 7;
        if (DEBUG) qDebug() << "start_x:" << start_x << "end_x:" << end_x;

        int rectX = widgetRect.left() + start_x * cellWidth;
        int rectY = widgetRect.top() + i * cellHeight;
        int rectWidth = (end_x - start_x + 1) * cellWidth;
        int rectHeight = cellHeight;
        QRectF scheduleRect(rectX, rectY, rectWidth, rectHeight);
        if (DEBUG) qDebug() << "rectX:" << rectX << "rectY:" << rectY
                 << "rectWidth:" << rectWidth << "rectHeight:" << rectHeight;

        QGraphicsRectItem* rectItem = scene->addRect(scheduleRect,
                                                     QPen(Qt::blue),
                                                     QBrush(Qt::Dense6Pattern));

        QGraphicsSimpleTextItem* textItem = new QGraphicsSimpleTextItem(sched.title, rectItem);
        QRectF textRect = textItem->boundingRect();
        qreal textX = scheduleRect.x() + (scheduleRect.width() - textRect.width()) / 2;
        qreal textY = scheduleRect.y() + (scheduleRect.height() - textRect.height()) / 2;
        textItem->setPos(textX, textY);
    }
}

void WeekBox::showEvent(QShowEvent* event)
{
    bool DEBUG = false;

    // 기본 show 이벤트 처리
    QGraphicsView::showEvent(event);

    // 위젯이 처음 보여질 때 한 번 sceneRect 업데이트
    scene->setSceneRect(0, 0, this->width(), this->height());
    if (DEBUG) qDebug() << "[WeekBox showEvent]" << this->width() << " " <<this->height();
    drawSchedules();
}

void WeekBox::resizeEvent(QResizeEvent* event)
{
    // 기본 resize 이벤트 처리
    QGraphicsView::resizeEvent(event);

    // 위젯의 새로운 크기에 맞춰 장면의 영역 설정
    scene->setSceneRect(0, 0, this->width(), this->height());
    drawSchedules();
}
