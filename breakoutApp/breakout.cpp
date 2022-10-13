#include <QLabel>
#include <QVector>
#include <QApplication>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFileInfo>

#include "breakout.h"
#include "ball.h"
#include "brick.h"
#include "paddle.h"

breakout::breakout(QWidget *parent)
    : QWidget{parent}, xDir(1), yDir(-1)
{
    ball = new Ball(this);      // 공 생성
    ball->setGeometry(SCR_WIDTH*0.8, SCR_HEIGHT*0.875, 20, 20);     // 공 시작 위치 설정
    //ball->setStyleSheet("QLabel {background-color:crimson; border-radius: 10px;}");

    paddle = new Paddle(this);      // 패들 생성
    paddle->setGeometry(SCR_WIDTH*0.7, SCR_HEIGHT*0.9, p_WIDTH, HEIGHT);        // 패들 시작 위치 설정
    //paddle->setStyleSheet("QLabel {background-color:lightcyan;}");

    gameOver = new QLabel("<font size = 100 color = red>Game Over</font>", this);   // 게임 오버시 뜨는 label
    gameOver->setGeometry(150, 200, 300, 400);      // 위치 설정
    gameOver->hide();       // 게임 진행 중에는 숨김 처리

    victory = new QLabel("<font size = 600 color = white>VICTORY</font>", this);    // 승리시 뜨는 label
    victory->setGeometry(163, 100, 200, 300);       // 위치 설정
    victory->hide();        // 게임 진행 중에는 숨김 처리

    for (int y= 0, i=0; y<5; y++)           // 벽돌을 6x5개 생성
        for (int x= 0; x<6; x++,i++) {
            bricks[i] = new Brick(this);
            //bricks[i]->setStyleSheet(static_cast<QString>(color.at(y)));
            //bricks[i]->setStyleSheet(color.at(y)+"border: 1px solid navy;}");
            bricks[i]->setGeometry(x*WIDTH, y*HEIGHT+30, WIDTH, HEIGHT);    // 벽돌을 x, y 좌표에 맞게 옮겨가며 생성
        }
    resize(SCR_WIDTH, SCR_HEIGHT);      // 게임 사이즈 설정(450x800)

    setMouseTracking(true);             // 마우스 따라다니기

    timerId = startTimer(4);            // 공 속도 설정

    /* 배경음악 출력을 위한 미디어 플레이어의 초기화 */
    QAudioOutput *bgAudioOutput = new QAudioOutput;
    bgAudioOutput->setVolume(10);

    bgPlayer = new QMediaPlayer( );             // 배경음악 플레이어
    bgPlayer->setAudioOutput(bgAudioOutput);
    bgPlayer->setLoops(QMediaPlayer::Infinite); /* 무한 반복 */
    bgPlayer->setSource(QUrl::fromLocalFile(QFileInfo("BackGround.mp3" ).absoluteFilePath( ))); // 절대경로 설정
    bgPlayer->play( );

    /* 효과음 출력을 위한 플레이어 */
    QAudioOutput *bgEffectOutput = new QAudioOutput;
    bgEffectOutput->setVolume(20);

    effectPlayer = new QMediaPlayer( );         // 효과음 플레이어
    effectPlayer->setAudioOutput(bgEffectOutput);
    effectPlayer->setLoops(QMediaPlayer::Once); // 충돌시 한 번만
    effectPlayer->setSource(QUrl::fromLocalFile(QFileInfo("effect.wav").absoluteFilePath( )));

    /* 게임 오버시 배경음 출력을 위한 플레이어 */
    QAudioOutput *gameOverOutput = new QAudioOutput;
    gameOverOutput->setVolume(20);

    gOverPlayer = new QMediaPlayer( );          // 게임오버 사운드 플레이어
    gOverPlayer->setAudioOutput(gameOverOutput);
    gOverPlayer->setLoops(QMediaPlayer::Once);  // 한번만
    gOverPlayer->setSource(QUrl::fromLocalFile(QFileInfo("GameOver.mp3").absoluteFilePath()));

    /* 승리시 배경음 출력을 위한 플레이어 */
    QAudioOutput *victoryOutput = new QAudioOutput;
    gameOverOutput->setVolume(30);

    victoryPlayer = new QMediaPlayer( );        // 승리 배경음 플레이어
    victoryPlayer->setAudioOutput(victoryOutput);
    victoryPlayer->setLoops(QMediaPlayer::Once);
    victoryPlayer->setSource(QUrl::fromLocalFile(QFileInfo("Victory.mp3").absoluteFilePath()));

}

breakout::~breakout()       // 메모리 누수 방지를 위한 delete
{
    delete ball;
    delete paddle;
    delete bgPlayer;
    delete effectPlayer;
    delete gOverPlayer;
    delete victoryPlayer;
    delete gameOver;
    delete victory;

    for (int i=0; i<NO_OF_BRICKS; i++) {    // 벽돌 30개 생성했기 때문에 하나씩 delete
        delete bricks[i];
    }
}

void breakout::keyPressEvent(QKeyEvent *e)  // 왼쪽, 오른쪽 방향키로 패들 움직이기
{
    switch (e->key()) {
    case Qt::Key_Left: {        // 왼쪽 방향키 눌렀을 때
        paddle->move(paddle->x()-MOVE_SPEED, paddle->y());  // 패들이 왼쪽으로 이동
        int x = paddle->x();
        x=(x<0)? 0:x;
        paddle->move(x, paddle->y());   // 패들이 화면 바깥으로 나가지 않도록 경계 검사
        break;
    }
    case Qt::Key_Right: {       // 오른쪽 방향키 눌렀을 때
        paddle->move(paddle->x()+MOVE_SPEED, paddle->y());  // 패들이 오른쪽으로 이동
        int x = paddle->x();
        x=(x<0)? 0:(x+p_WIDTH > width())? width()-p_WIDTH:x;    // 패들이 화면 바깥으로 나가지 않도록 경계 검사
        paddle->move(x, paddle->y());
        break;
    }
    case Qt::Key_Escape:    // esc 누르면 종료
        qApp->exit();
        break;
    default:
        QWidget::keyPressEvent(e);  // 왼쪽, 오른쪽, esc 이외의 키는 대기
    }
}

void breakout::mouseMoveEvent(QMouseEvent *e)   // 마우스로 패들 움직이기
{
    int x= e->pos().x();
    x=(x<0)? 0:(x+p_WIDTH > width())? width()-p_WIDTH:x;    // 패들이 화면 밖으로 나가는 것 방지
    paddle->move(x, paddle->y());
}

void breakout::timerEvent(QTimerEvent *e)   // 공이 움직일 때 이벤트 처리
{
    Q_UNUSED(e);
    moveObjects();      // 공이 패들과 벽돌을 맞으면 반대 방향으로 날라감
    checkCollision();   // 공이 벽돌을 만나면 벽돌을 파괴
}

void breakout::moveObjects()
{
    ball->move(ball->x() + xDir, ball->y() + yDir);

    if ((ball->x() <=0)||(ball->x()+10 >= SCR_WIDTH))
    {
        xDir *= -1;
    }

    if (ball->y() <=0)
        yDir = 1;
}

void breakout::checkCollision()
{
    for (int i=0; i<NO_OF_BRICKS; i++) {
        if ((ball->geometry()).intersects(bricks[i]->geometry())) {
            int ballLeft = ball->geometry().left();         //왼쪽
            int ballHeight = ball->geometry().height();     //아래
            int ballWidth = ball->geometry().width();       //오른쪽
            int ballTop = ball->geometry().top();           //위
            // 현재 블록의 위치를 계산
            QPoint pointRight(ballLeft + ballWidth + 1, ballTop);
            QPoint pointLeft(ballLeft-1, ballTop);
            QPoint pointTop(ballLeft, ballTop-1);
            QPoint pointBottom(ballLeft, ballTop + ballHeight +1);

            // 벽돌이 아직 남아 있을 때
            if (!bricks[i]->isHidden()) {
                if(bricks[i]->geometry().contains(pointRight))
                    xDir = -1;
                else if(bricks[i]->geometry().contains(pointLeft))
                    xDir = 1;
                if(bricks[i]->geometry().contains(pointTop))
                    yDir = 1;
                else if(bricks[i]->geometry().contains(pointBottom))
                    yDir = -1;
                // 벽돌에 공이 부딪히면 반대방향으로 날아가고 벽돌을 숨김
                bricks[i]->setHidden(true);
                effectPlayer->play();       // 효과음 재생
            }
        }
    }

    if (ball->geometry().bottom() > height()) {     // 공이 바닥으로 떨어질 경우
        killTimer(timerId);     // 이벤트 멈춤
        gameOver->show();       // 게임 오버 표시
        bgPlayer->stop();
        gOverPlayer->play();    // 게임 오버 재생
        qDebug("Game Over");

    }

    int j = 0;
    for (int i =0; i < NO_OF_BRICKS; i++)       //
        if (bricks[i]->isHidden())
            j++;

    if (j==NO_OF_BRICKS) {      // 벽돌을 다 깨면
        killTimer(timerId);     // 이벤트 멈춤
        victory->show();        // 승리 표시
        bgPlayer->stop();
        victoryPlayer->play();  // 승리 효과음 재생
        qDebug("Victory");
    }


    //패들과 충돌을 처리
    if ((ball->geometry()).intersects(paddle->geometry())) {
        int paddleLPos = paddle->geometry().left();     // 패들의 맨 왼쪽
        int ballLPos = ball->geometry().left();         // 공의 맨 왼쪽
        int first = paddleLPos + 8;                     // 패들을 구간별로 나누기
        int second = paddleLPos + 16;
        int third = paddleLPos + 24;
        int fourth = paddleLPos + 32;

        if(ballLPos < first) {      // 패들의 맨 왼쪽을 맞을 때
            xDir =-1; yDir=-1;
        }
        if(ballLPos >= first && ballLPos < second) {    // 패들의 1/3 지점에 맞을 때
            xDir = -1; yDir *= -1;
        }
        if(ballLPos >= second && ballLPos < third) {    // 패들의 1/2 지점에 맞을 때
            xDir =0; yDir = -1;
        }
        if(ballLPos >= third && ballLPos < fourth) {    // 패들의 2/3 지점에 맞을 때
                xDir =1; yDir *= -1;
        }
        if(ballLPos > fourth) {                         // 패들의 오른쪽에 맞을 때
            xDir =1; yDir =-1;
        }
    }
}


