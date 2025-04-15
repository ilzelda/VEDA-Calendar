#include "weekbox.h"
#include <QPainter>
#include <QtGlobal> // qMin 사용

WeekBox::WeekBox(int n, QWidget *parent)
    : QGraphicsView(parent), nth_week(n)
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

    min_day = n * 7 + 1;
    max_day = min_day + 6;
}

WeekBox::~WeekBox()
{
}

void WeekBox::drawSchedules()
{
    // bool DEBUG = true;
    bool DEBUG = false;
    qDebug()<<"Rendering"<<nth_week<< "th week : ";

    scene->clear();
    // 위젯 전체 영역 (QGraphicsView의 viewport 크기를 사용)
    QRectF widgetRect(0, 0, this->width() - 1, this->height() - 1);

    // 디버그용 경계선을 추가 (원래 paintEvent에서 그리던 테두리)
    if (DEBUG)
        scene->addRect(widgetRect, QPen(Qt::black), QBrush(Qt::NoBrush));

    // 가로 7등분: 각 칸의 너비 계산
    int cellWidth = widgetRect.width() / 7;

    // 세로 높이 결정:
    int scheduleCount = (schedule_list.size() > 0) ? schedule_list.size() : 1; // 스케줄 없으면 1
    int cellHeight = std::min(static_cast<int>(widgetRect.height() / scheduleCount),
                              static_cast<int>(widgetRect.height() / 2));

    // 각 스케줄마다 사각형과 텍스트 아이템 생성
    for (int i = 0; i < schedule_list.size(); i++) {
        const Schedule& sched = schedule_list.at(i);

        // 날짜 계산 (실제 코드에서는 날짜 계산 로직을 상황에 맞게 조정)
        int start_day = std::max(sched.start.date().day(), min_day);
        int end_day = std::min(sched.end.date().day(), max_day);
        qDebug() << "start_day:" << start_day << "end_day:" << end_day;

        int start_x = (start_day - 1) % 7;
        int end_x = (end_day - 1) % 7;
        qDebug() << "start_x:" << start_x << "end_x:" << end_x;

        // 사각형 위치와 크기 계산
        int rectX = widgetRect.left() + start_x * cellWidth;
        int rectY = widgetRect.top() + i * cellHeight;
        int rectWidth = (end_x - start_x + 1) * cellWidth;
        int rectHeight = cellHeight;
        QRectF scheduleRect(rectX, rectY, rectWidth, rectHeight);
        qDebug() << "rectX:" << rectX << "rectY:" << rectY
                 << "rectWidth:" << rectWidth << "rectHeight:" << rectHeight;

        // QGraphicsRectItem을 생성 (채움은 CrossPattern)
        QGraphicsRectItem* rectItem = scene->addRect(scheduleRect,
                                                     QPen(Qt::blue),
                                                     QBrush(Qt::Dense6Pattern));

        // 사각형 중앙에 텍스트 표시를 위해 QGraphicsSimpleTextItem 추가.
        // rectItem을 부모로 설정하면 함께 이동하는 장점이 있음.
        QGraphicsSimpleTextItem* textItem = new QGraphicsSimpleTextItem(sched.title, rectItem);
        QRectF textRect = textItem->boundingRect();
        qreal textX = scheduleRect.x() + (scheduleRect.width() - textRect.width()) / 2;
        qreal textY = scheduleRect.y() + (scheduleRect.height() - textRect.height()) / 2;
        textItem->setPos(textX, textY);
    }
}

void WeekBox::showEvent(QShowEvent* event)
{
    // 기본 show 이벤트 처리
    QGraphicsView::showEvent(event);

    // 위젯이 처음 보여질 때 한 번 sceneRect 업데이트
    scene->setSceneRect(0, 0, this->width(), this->height());
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


void WeekBox::mousePressEvent(QMouseEvent* event)
{
    // 클릭한 위치에 QGraphicsItem(예, schedule rect)이 있는지 확인
    QGraphicsItem* clickedItem = itemAt(event->pos());
    if (clickedItem) {
        // 아이템이 있으면 기본 처리 (schedule rect 아이템의 이벤트 처리)
        qDebug() << "Graphics Item이 클릭됨";
        QGraphicsView::mousePressEvent(event);
    } else {
        qDebug() << "Graphics Item이 클릭되지 않음";
        // 빈 영역이면 먼저 전역 좌표에서 위젯을 구합니다.
        QWidget* target = QApplication::widgetAt(event->globalPosition().toPoint());
        // 만약 target이 WeekBox(self)라면,
        // 부모(컨테이너)의 childAt()을 이용하여 실제 대상 위젯을 찾아봅니다.
        if (target == this && parentWidget()) {
            // 부모 위젯의 좌표계로 전환
            QPoint containerPos = parentWidget()->mapFromGlobal(event->globalPosition().toPoint());
            target = parentWidget()->childAt(containerPos);
        }
        // target이 유효하고 WeekBox가 아니라면 이벤트를 전달
        if (target && target != this) {
            qDebug() << "부모의 위젯에서 target을 찾음(" << target->metaObject()->className() <<")";

            QPoint localPos = target->mapFromGlobal(event->globalPosition().toPoint());
            // 새 마우스 이벤트를 생성하여 비동기적으로 전달합니다.
            QMouseEvent* newEvent = new QMouseEvent(event->type(),
                                                    localPos,
                                                    event->globalPosition().toPoint(),
                                                    event->button(),
                                                    event->buttons(),
                                                    event->modifiers());
            QCoreApplication::postEvent(target, newEvent);
        }
        // 현재 위젯에서는 이벤트를 처리하지 않음을 명시
        event->ignore();
    }
}

void WeekBox::mouseMoveEvent(QMouseEvent* event)
{
    // 마우스 프레스 이벤트 재정의: 클릭이 사각형 아이템 위에 있으면 그대로 처리하고,
    // 빈 영역이면 뒷쪽 위젯에 이벤트를 전달합니다.

    // QGraphicsView의 좌표를 사용해서 클릭한 아이템 검색
    QGraphicsItem* clickedItem = itemAt(event->pos());
    if (clickedItem)
    {
        // 아이템이 있으면 기본 처리(예를 들어, 아이템에 등록된 이벤트 핸들러가 실행됨)
        QGraphicsView::mouseMoveEvent(event);
    }
    else
    {
        // 아이템이 없는 빈 영역이면, 클릭 이벤트를 뒷쪽 위젯으로 전달합니다.
        // 전역 좌표를 사용하여 그 위치의 위젯 찾기
        QWidget* target = QApplication::widgetAt(event->globalPosition().toPoint());

        // 단, target이 현재 위젯(WeekBox) 자체가 아니어야 합니다.
        if (target && target != this) {
            // 대상 위젯의 좌표계로 이벤트 좌표를 변환
            QPoint localPos = target->mapFromGlobal(event->globalPosition().toPoint());
            QMouseEvent* newEvent = new QMouseEvent (event->type(),
                                                    localPos,
                                                    event->globalPosition(),
                                                    event->button(),
                                                    event->buttons(),
                                                    event->modifiers());
            // 동기적으로 대상 위젯에 이벤트 전달
            QCoreApplication::postEvent(target, newEvent);
        }
        // 현재 위젯에서는 이벤트를 처리하지 않음
        event->ignore();
    }
}

void WeekBox::mouseReleaseEvent(QMouseEvent* event)
{
    // 마우스 프레스 이벤트 재정의: 클릭이 사각형 아이템 위에 있으면 그대로 처리하고,
    // 빈 영역이면 뒷쪽 위젯에 이벤트를 전달합니다.

    // QGraphicsView의 좌표를 사용해서 클릭한 아이템 검색
    QGraphicsItem* clickedItem = itemAt(event->pos());
    if (clickedItem)
    {
        // 아이템이 있으면 기본 처리(예를 들어, 아이템에 등록된 이벤트 핸들러가 실행됨)
        QGraphicsView::mouseReleaseEvent(event);
    }
    else
    {
        // 아이템이 없는 빈 영역이면, 클릭 이벤트를 뒷쪽 위젯으로 전달합니다.
        // 전역 좌표를 사용하여 그 위치의 위젯 찾기
        QWidget* target = QApplication::widgetAt(event->globalPosition().toPoint());

        // 단, target이 현재 위젯(WeekBox) 자체가 아니어야 합니다.
        if (target && target != this) {
            // 대상 위젯의 좌표계로 이벤트 좌표를 변환
            QPoint localPos = target->mapFromGlobal(event->globalPosition().toPoint());
            QMouseEvent* newEvent = new QMouseEvent (event->type(),
                                                    localPos,
                                                    event->globalPosition(),
                                                    event->button(),
                                                    event->buttons(),
                                                    event->modifiers());
            // 동기적으로 대상 위젯에 이벤트 전달
            QCoreApplication::postEvent(target, newEvent);
        }
        // 현재 위젯에서는 이벤트를 처리하지 않음
        event->ignore();
    }
}
// 필요에 따라 mouseReleaseEvent, mouseMoveEvent 등도 같은 방식으로 재정의할 수 있습니다.
