#include "ball.h"
#include <QPainter>

#define RADIUS 10               // 반지름 매크로 설정

Ball::Ball(QWidget *parent)     // 게임에 사용하는 공
    : QWidget{parent}
{
    resize(RADIUS, RADIUS);         /* 공을 이미지로 처리 */
    //setPixmap(QPixmap("ball.png"));
    //setStyleSheet("background-color: rgba(0, 0, 0, 0");
}

void Ball::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter p(this);                       // 공 스타일 설정
    p.setPen(QColor(Qt::transparent));      // 공 투명도
    p.setBrush(QColor(Qt::red));            // 공 색깔
    p.drawEllipse(0, 0, RADIUS, RADIUS);    // 공 그리기
}


