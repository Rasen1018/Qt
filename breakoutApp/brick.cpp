#include <QPainter>
#include "brick.h"

#define WIDTH 75                // 벽돌 길이
#define HEIGHT 18               // 벽돌 높이

Brick::Brick(QWidget *parent)
    : QWidget{parent}
{
    resize(WIDTH, HEIGHT);      /* 벽돌을 이미지로 처리 */
    //setPixmap(QPixmap("brick.png").scaled(WIDTH, HEIGHT));
    //setStyleSheet("background-color: rgba(0,0,0,0");
}

void Brick::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter p(this);                                   // 벽돌 스타일 설정
    p.setBrush(QColor(QColorConstants::Svg::salmon));   // 벽돌 색깔
    p.drawRect(0, 0, WIDTH, HEIGHT);                    // 직사각형으로 그리기
}
