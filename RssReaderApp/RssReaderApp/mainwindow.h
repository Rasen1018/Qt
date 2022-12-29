#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QNetworkReply>
#include <QListView>
#include <QStandardItemModel>
#include <QProgressBar>
#include <QWebEngineView>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openRssFeed( );
    void replyFinished(QNetworkReply*);
    void listViewDoubleClicked(const QModelIndex& index);
    void downloadProgress(qint64 bytes, qint64 bytesTotal);

private:
    QListView* tv;
    QStandardItemModel* model;
    QComboBox *combo;
    QNetworkAccessManager *manager;
    QWebEngineView* wv;
    QProgressBar* progress;
};

class ListView : public QListView {
    Q_OBJECT

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H
