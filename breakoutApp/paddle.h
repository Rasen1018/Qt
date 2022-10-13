#ifndef PADDLE_H
#define PADDLE_H

#include <QWidget>

class Paddle : public QWidget {

    Q_OBJECT
public:
    explicit Paddle(QWidget *parent = nullptr);

signals:

private:
    void paintEvent(QPaintEvent *e);

};

#endif // PADDLE_H
