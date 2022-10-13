#ifndef BREAKOUT_H
#define BREAKOUT_H

#include <QMouseEvent>
#include <QKeyEvent>
#include <QWidget>

class QMediaPlayer;
class QLabel;
class Ball;
class Brick;
class Paddle;

class breakout : public QWidget
{
public:
    explicit breakout(QWidget *parent = nullptr);
    ~breakout();
protected:
    void keyPressEvent(QKeyEvent *);
    void mouseMoveEvent(QMouseEvent *e);
    void timerEvent(QTimerEvent *);
    void moveObjects();
    void checkCollision();

protected:
    static const int MOVE_SPEED = 8;

private:
    static const int NO_OF_BRICKS=30;
    const int WIDTH =75;
    const int HEIGHT = 18;
    const int p_WIDTH=WIDTH*1.5;
    const int SCR_WIDTH = 450;
    const int SCR_HEIGHT = 800;
    int timerId;
    int xDir, yDir;

    Ball *ball;
    Paddle *paddle;
    Brick *bricks[NO_OF_BRICKS];
    QLabel *gameOver, *victory;
    QMediaPlayer *bgPlayer, *effectPlayer, *gOverPlayer, *victoryPlayer;
signals:

};

#endif // BREAKOUT_H
