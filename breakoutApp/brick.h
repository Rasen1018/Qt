#ifndef BRICK_H
#define BRICK_H

#include <QWidget>

class Brick : public QWidget
{
    Q_OBJECT
public:
    explicit Brick(QWidget *parent = nullptr);

signals:

private:
    void paintEvent(QPaintEvent *e);

};

#endif // BRICK_H
