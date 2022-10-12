#ifndef QTEDITOR_H
#define QTEDITOR_H

#include <QMainWindow>
#include <QHash>

class QTextEdit;
class QMdiArea;
class QAction;
class QMdiSubWindow;
class QMenu;


class QtEditor : public QMainWindow
{
    Q_OBJECT

public:
    QtEditor(QWidget *parent = nullptr);
    ~QtEditor();


private:
    QList<QAction*> actions;
    QHash<QAction*, QWidget*> windowHash;
    QMenu *windowMenu;
    QTextEdit* prevTextEdit = nullptr;  // 초기화 필요
    QMdiArea* mdiArea;
    bool eventFilter(QObject*, QEvent*);

#if 1
    QAction *makeAction(QString icon, QString name, QString shortCut, QString toolTip, QObject* recv, const char* slot);
    QAction *makeAction(QString name, QString shortCut, QString toolTip, QObject* recv, const char* slot);
    QAction *makeAction(QString icon, QString name, QString shortCut, QString toolTip);

#else
private:
    template <typename T>
    QAction* makeAction(QString icon, QString text, T shortCut,
                        QString toolTip, QObject* rect, const char* slot);
    template <>
    QAction* makeAction(QString icon, QString text, const char* shortCut,
                        QString toolTip, QObject* rect, const char* slot);

    template <typename T, typename Functor>
    QAction* makeAction(QString icon, QString text, T shortCut,
                        QString toolTip, Functor lambda);

#endif
public slots:
    void connectWindow(QMdiSubWindow*);
    void setFontWidget();
    void setTextFont(QFont);
    void setTextSize(qreal);
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void printt();
    void selectWindow();
    void closeWindow();
    void setColor();
    void setFontt();
    void about();
};
#endif // QTEDITOR_H

