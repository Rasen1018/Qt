#ifndef BALL_H
#define BALL_H

#include <QWidget>

class Ball : public QWidget
{
    Q_OBJECT
public:
    explicit Ball(QWidget *parent = nullptr);

signals:

private:
    void paintEvent(QPaintEvent *e);

};

#endif // BALL_H
