#include <QApplication>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QKeySequence>
#include <QTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include <QStatusBar>
#include <QAction>
#include <QToolBar>
#include <QFileDialog>
#include <QFontComboBox>
#include <QDoubleSpinBox>
#include <QDockWidget>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QKeyEvent>
#include "qteditor.h"

QtEditor::QtEditor(QWidget *parent)
    : QMainWindow(parent) // 부모의 생성자
{
    mdiArea=new QMdiArea(this);
    setCentralWidget(mdiArea);
#if 0
    QTextEdit* textedit = new QTextEdit(this);        // text edit 생성
    //setCentralWidget(textedit);                     // widget과 크기 동기화
    mdiArea->addSubWindow(textedit);
#else
#endif
    QMenuBar *menubar = new QMenuBar(this);         // menubar 생성
    setMenuBar(menubar);                            // menubar 선언

    qDebug("1");
/*================================== FILE MENU =======================================*/

    QAction *newAct = new QAction(QIcon(":/image/new.png"), "&New", this);      // menu에 넣을 서브 메뉴 생성
    newAct->setShortcut(tr("Ctrl+N"));                                          // 숏컷 지정
    newAct->setStatusTip(tr("Make new file"));
    connect(newAct, SIGNAL(triggered()), SLOT(newFile()));

    // next window lambda 사용 : connect(mdiArea, QKeySequence::MoveToPreviousChar, mdiArea, [=](){mdiArea->activateNextSubWindow();});

    QAction *exitAct = new QAction(QIcon(":/image/exit.png"), "E&xit", this);       // 창 끄기 옵션
    exitAct->setShortcut(tr("Ctrl+Q"));                                             // 단축키 설정
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(quit()));                      // qApp에서 quit

    QAction *openAct = makeAction(":/image/open.png","&Open",
                                  "Ctrl+O", "Open any files", this, SLOT(openFile()));  // 파일 열기
    QAction *saveAct = makeAction("&Save", "Ctrl+S", "Save this file",
                                  this, SLOT(saveFile()));                              // 파일 저장
    QAction *saveAsAct = makeAction(":/image/save.png", "Save as", "Ctrl+Shift+S",
                                    "Save another name", this, SLOT(saveAsFile()));     // 파일 다른이름 저장
    QAction *printAct = makeAction("&Print", "Ctrl+P", "This file prints",
                                   this, SLOT(printt()));                               // 파일 인쇄

    QMenu *fileMenu = menubar->addMenu("&File");        // 파일 메뉴에 서브 메뉴 추가
    fileMenu->addAction(newAct);                        // 파일 메뉴에 action 추가
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    qDebug("2");
/*================================== EDIT MENU =======================================*/

    QMenu *editMenu = menubar->addMenu("&Edit");        // edit 메뉴 구현

    QAction *undoAct = makeAction(":/image/Undo.png", "Undo", "Ctrl+Alt+Z", "Undo");            //Edit 기능 구현, 함수 오버로딩
    QAction *redoAct = makeAction(":/image/Redo.png", "Redo", "Ctrl+Alt+Y", "Redo");
    QAction *copyAct = makeAction(":/image/Copy.png", "Copy", "Ctrl+Shift+C", "Copy");
    QAction *cutAct = makeAction(":/image/Cut.png", "Cut", "Ctrl+Shift+X", "Cut");
    QAction *pasteAct = makeAction(":/image/Paste.png", "Paste", "Ctrl+Shift+V", "Paste");
    QAction *zoomInAct = makeAction(":/image/zoomIn.png", "Zoom In", "Ctrl+,", "ZoomIn");
    QAction *zoomOutAct = makeAction(":/image/zoomOut.png", "Zoom Out", "Ctrl+.", "ZoomOut");

    editMenu->addAction(undoAct);       // 에딧 메뉴에 action 추가
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(copyAct);
    editMenu->addAction(cutAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(zoomInAct);
    editMenu->addAction(zoomOutAct);

    actions.append(undoAct);			/* QList에 QAction들을 추가 */
    actions.append(redoAct);
    actions.append(copyAct);
    actions.append(cutAct);
    actions.append(pasteAct);
    actions.append(zoomInAct);
    actions.append(zoomOutAct);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            SLOT(connectWindow(QMdiSubWindow*)));

    qDebug("3");
/*================================== WINDOW MENU =======================================*/

    QMenu *formatMenu = menubar->addMenu("Format");         // format 메뉴 구현
    windowMenu = menubar->addMenu("&Window");               // window 메뉴 구현

    QAction *cascadeAct = makeAction("Cascade", "Ctrl+Alt+C",
                                      "Sort windows to cascade", mdiArea, SLOT(cascadeSubWindows()));   // 정렬 기능
    QAction *tileAct = makeAction("Tile", "Ctrl+Alt+T",
                                  "Sort windows to cascade", mdiArea, SLOT(tileSubWindows()));
    QAction *prevAct = makeAction("Prev", "Alt+Left", "Select previous window",
                                  mdiArea, SLOT(activatePreviousSubWindow()));      // 전 윈도우 선택
    QAction *nextAct = makeAction("Next", "Alt+Right", "Select next window",
                                  mdiArea, SLOT(activateNextSubWindow()));          // 다음 윈도우 선택
    QAction *closeAct = makeAction("Close", "Ctrl+J", "Close current window",
                                   mdiArea, SLOT(closeActiveSubWindow()));

    QLabel* docklabel = new QLabel("Dock Widget", this);        // dock widget
    QDockWidget *dock = new QDockWidget("Dock Widget", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);               // 오른쪽에 dock widget 생성
    dock->setWidget(docklabel);

    windowMenu->addAction(cascadeAct);      // window 메뉴에 action 추가
    windowMenu->addAction(tileAct);
    windowMenu->addAction(prevAct);
    windowMenu->addAction(nextAct);
    windowMenu->addAction(closeAct);
    windowMenu->addAction(dock->toggleViewAction());
    newFile();

    qDebug("4");
/*================================== FORMAT MENU =======================================*/

    QMenu *alignMenu = new QMenu("Ali&gn", this);           // align 메뉴 구현

    QAction *fontColor = makeAction("Font Color", "", "Set font color", this, SLOT(setColor()));
    QAction *fontKind = makeAction("Font", "", "set font feature", this, SLOT(setFontt()));

    formatMenu->addAction(fontKind);
    formatMenu->addAction(fontColor);
    formatMenu->addMenu(alignMenu);                         // format 메뉴 안에 align "메뉴" 추가

    QAction *alignCenterAct = new QAction(QIcon(":/image/aligncenter.png"), "Center", this);  // center action 생성
    connect(alignCenterAct, &QAction::triggered, this, [=](){       // 외부 값을 가져옴, textedit에 있는 setAlign을 불러서 정렬
        QMdiSubWindow *subWindow = mdiArea->currentSubWindow();     // mdiArea 안에 각각 subwindow를 지정
        QTextEdit *textedit = dynamic_cast<QTextEdit*>(subWindow->widget());    // 각각 subwindow의 위젯을 textedit으로 형변환
        textedit->setAlignment(Qt::AlignCenter);});  // lambda 사용, 가운데 정렬

    QAction *alignLeftAct = new QAction(QIcon(":/image/alignleft.png"), "Left", this);      // left action 생성
    connect(alignLeftAct, &QAction::triggered, this, [=](){
        QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
        QTextEdit *textedit = dynamic_cast<QTextEdit*>(subWindow->widget());
        textedit->setAlignment(Qt::AlignLeft);});       // 왼쪽 정렬

    QAction *alignRightAct = new QAction(QIcon(":/image/alignright.png"), "Right", this);    // right action 생성
    connect(alignRightAct, &QAction::triggered, this, [=](){
        QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
        QTextEdit *textedit = dynamic_cast<QTextEdit*>(subWindow->widget());
        textedit->setAlignment(Qt::AlignRight);});      // 오른쪽 정렬

    QAction *alignJustifyAct = new QAction(QIcon(":/image/alignjustify.png"), "Justify", this);    // justify action 생성
    connect(alignJustifyAct, &QAction::triggered, this, [=](){
        QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
        QTextEdit *textedit = dynamic_cast<QTextEdit*>(subWindow->widget());
        textedit->setAlignment(Qt::AlignJustify);});    // 양쪽 정렬

    alignMenu->addAction(alignCenterAct);   // action 추가
    alignMenu->addAction(alignLeftAct);
    alignMenu->addAction(alignRightAct);
    alignMenu->addAction(alignJustifyAct);

    qDebug("5");
/*================================== HELP MENU =======================================*/

    QMenu *helpMenu = menubar->addMenu("&Help");                // help 메뉴 구현
    QAction *aboutAct = new QAction("About", this);
    connect(aboutAct, SIGNAL(triggered()), SLOT(about()));

    QAction *qtAct = new QAction("About Qt", this);
    connect(qtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    helpMenu->addAction(aboutAct);
    helpMenu->addAction(qtAct);

    qDebug("6");
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ TOOL BAR ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
#if 0
    QToolBar *fileToolBar = addToolBar("File");                 // 툴바 생성
    fileToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);    // 툴바 표시 스타일(아이콘만)
    fileToolBar->addAction(openAct);                            // 툴바에 액션 추가
    fileToolBar->addSeparator();                                // 툴바 구분선 표시
    fileToolBar->addAction(saveAsAct);

    addToolBarBreak();      // 툴바 줄바꿈
    QToolBar *filetoolbar = addToolBar("File2");                // 툴바 생성
    filetoolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    filetoolbar->addAction(newAct);
    filetoolbar->addSeparator();
    filetoolbar->addAction(exitAct);



    windowMenu->addAction(fileToolBar->toggleViewAction());     // 툴바 추가
    windowMenu->addAction(filetoolbar->toggleViewAction());     // 툴바 추가
#else
/*================================== EDIT TOOL BAR ========================================*/

    QToolBar *editToolbar = addToolBar("TextEdit");             // edit menu에 있는 action을 툴바에 설정
    editToolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);    // toolbar에 icon만 표시
    editToolbar->addAction(newAct);
    editToolbar->addSeparator();         // 경계 나누기
    editToolbar->addAction(undoAct);
    editToolbar->addAction(redoAct);
    editToolbar->addSeparator();
    editToolbar->addAction(cutAct);
    editToolbar->addAction(copyAct);
    editToolbar->addAction(pasteAct);
    editToolbar->addSeparator();
    editToolbar->addAction(zoomInAct);
    editToolbar->addAction(zoomOutAct);

#endif
    QFontComboBox* fontBox = new QFontComboBox(this);      // 폰트 툴바 만들기
    //connect(font, SIGNAL(currentFontChanged(QFont)), SLOT(setTextFont(QFont)));
    connect(fontBox, &QFontComboBox::currentFontChanged, this, [=](QFont font1){   // QFont를 인자로 받아와서 사용
        QMdiSubWindow *subWindow = mdiArea->currentSubWindow();                 // mdiArea 안에 각각 subwindow를 지정
        QTextEdit *textedit = dynamic_cast<QTextEdit*>(subWindow->widget());    // 각각 subwindow의 위젯을 QtextEdit 포인터로 형변환
        textedit->setCurrentFont(font1);                                        // textedit의 현재 폰트에 위젯 연결
    });
    connect(mdiArea, &QMdiArea::subWindowActivated, this, [=](QMdiSubWindow*){
        QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
        if(subWindow==nullptr) return;
        QTextEdit *textedit = dynamic_cast<QTextEdit*>(subWindow->widget());
        QFont font = textedit->currentFont();
        fontBox->setCurrentFont(font);
    });

    QDoubleSpinBox* spinBox = new QDoubleSpinBox(this);    // 글씨 크기 툴바 만들기
    connect(spinBox, &QDoubleSpinBox::valueChanged, this, [=](qreal size){         // qreal(double)를 인자로 받아와서 사용
        QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
        QTextEdit *textedit = dynamic_cast<QTextEdit*>(subWindow->widget());
        QFont font1 = textedit->currentFont();      // 현재 폰트 저장
        font1.setPointSizeF(size);                  // 폰트 사이즈 변경
        textedit->setCurrentFont(font1);            // 변경한 폰트 사이즈 현재 위젯에 연결
    });
    connect(mdiArea, &QMdiArea::subWindowActivated, this, [=](QMdiSubWindow*){
        QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
        if(subWindow==nullptr) return;
        QTextEdit *textedit = dynamic_cast<QTextEdit*>(subWindow->widget());
        QFont font = textedit->currentFont();
        spinBox->setValue(font.pointSizeF());
    });

    qDebug("7");
/*================================== FORMAT TOOL BAR ========================================*/

    addToolBarBreak();      // 툴바 줄바꿈
    QToolBar* formatToolbar = addToolBar("Format");
    formatToolbar->addWidget(fontBox);
    formatToolbar->addWidget(spinBox);
    formatToolbar->addSeparator();
    formatToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);     // icon 아래에 text 표시
    formatToolbar->addAction(alignJustifyAct);
    formatToolbar->addAction(alignLeftAct);
    formatToolbar->addAction(alignCenterAct);
    formatToolbar->addAction(alignRightAct);

    qDebug("8");
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ another fuction ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

    QStatusBar* statusbar = statusBar();       // tooltip 표시
    QLabel* statusLabel = new QLabel("Qt Editor", statusbar);
    statusLabel->setObjectName("StatusLabel");
    statusbar->addPermanentWidget(statusLabel);
    statusbar->showMessage("stared", 1500);

    qDebug("9");
}

/*****************************************************************************************************/

void QtEditor::connectWindow(QMdiSubWindow* window)     // 각각 subwindow마다 edit action 작동시키기 (connect, disconnect 이용)
{
    if(window == nullptr) {         // 새 창을 끄면 프로그램이 죽는 것 방지
        prevTextEdit = nullptr;     // 창이 없는 상태에서 new file 실행 시 프로그램 죽는 것 방지
        return;
    }
    QTextEdit *textEdit = qobject_cast<QTextEdit*>(window->widget( ));
    if(prevTextEdit != nullptr)
        Q_FOREACH(QAction *action, actions)		/* 모든 QAction에 연결된 슬롯 해제 */
            action->disconnect(prevTextEdit);
    prevTextEdit = textEdit;                    /* 현재 객체를 다음 처리를 위해 저장 */

    // 슬롯들을 위한 배열
    const char *methods[7] = {
        SLOT(undo( )), SLOT(redo( )), SLOT(copy( )), SLOT(cut( )),
        SLOT(paste( )), SLOT(zoomIn( )), SLOT(zoomOut( ))
    };

    /* QAction에 현재 선택된 객체의 슬롯을 연결 */
    int cnt = 0;
    Q_FOREACH(QAction *action, actions) {
        connect(action, SIGNAL(triggered()), textEdit, methods[cnt++]);
    }
}

bool QtEditor::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        qDebug("Key press %d", keyEvent->key());
        return false;
    }
    else {
        return QObject::eventFilter(obj, event);
    }
}

QtEditor::~QtEditor()     // 소멸자
{
}

void QtEditor::newFile() {      // 새 파일 만들기 슬롯
    qDebug("Make New File");
    QAction* windowAct = new QAction("New File", this);
    windowMenu->addAction(windowAct);
    QTextEdit *textedit = new QTextEdit;
    connect(textedit, SIGNAL(destroyed(QObject*)), windowAct, SLOT(deleteLater()));
    //connect(textedit, SIGNAL(destroyed(QObject*)), this, SLOT(closeWindow()));
    connect(textedit, SIGNAL(cursorPositionChanged()), this, SLOT(setFontWidget()));
    mdiArea->addSubWindow(textedit);
    windowHash[windowAct] = textedit;
    connect(windowAct, SIGNAL(triggered()), SLOT(selectWindow()));
    textedit->installEventFilter(this);
    textedit->show();
}

void QtEditor::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select file to open", ".",
                                                    "Text File (*.txt *.html *.c *.cpp *.h)");
    qDebug() << filename;

    QFileInfo fileInfo(filename);
    if(fileInfo.isReadable()) {
        QFile *file = new QFile(filename);
        file->open(QIODevice::ReadOnly);
        QByteArray msg = file->readAll();
        file->close();
        delete file;
        QTextEdit* textedit=new QTextEdit(this);
        textedit->setWindowTitle(filename);
        textedit->setHtml(msg);
        mdiArea->addSubWindow(textedit);

        auto stringList = filename.split(QLatin1Char('/'));
        QString opename = stringList[stringList.size()-2]+'/'+stringList[stringList.size()-1];

        QAction* windowAct = new QAction(opename, this);
        windowMenu->addAction(windowAct);
        windowHash[windowAct] = textedit;
        connect(windowAct, SIGNAL(triggered()), SLOT(selectWindow()));
        connect(textedit, SIGNAL(destroyed(QObject*)), windowAct, SLOT(deleteLater()));
        textedit->show();
    }
    else {
        QMessageBox::warning(this, "Error", "Can't Read this file", QMessageBox::Cancel);
    }

}

void QtEditor::saveFile()
{
    QMdiSubWindow *subwindow = mdiArea->currentSubWindow();
    if(subwindow==nullptr) return;
    QTextEdit *textedit = dynamic_cast<QTextEdit*>(subwindow->widget());
    QString filename = textedit->windowTitle();
    windowHash.key(textedit)->setText(filename);

    if(!filename.length()) {
        filename = QFileDialog::getSaveFileName(this, "Select file to save", ".",
                                                "Text File (*.txt *.html *.c *.cpp *.h)");
        textedit->setWindowTitle(filename);
    }
    qDebug() << filename;
    QFile *file = new QFile(filename);
    file->open(QIODevice::WriteOnly|QIODevice::Text);
    QFileInfo fileInfo(filename);
    if(fileInfo.isWritable()) {
        QByteArray msg = textedit->toHtml( ).toUtf8( );   // QString을 QByteArray로 변환
        file->write(msg);
    }
    else {
        QMessageBox::warning(this, "Error", "Can't Save this file", QMessageBox::Ok);
    }
    file->close();
    delete file;
}

void QtEditor::saveAsFile()
{
    QMdiSubWindow *subwindow = mdiArea->currentSubWindow();
    if(subwindow==nullptr) return;
    QTextEdit *textedit = dynamic_cast<QTextEdit*>(subwindow->widget());
    QString filename = QFileDialog::getSaveFileName(this, "Select file to save as", ".",
                                                    "Text File (*.txt *.html *.c *.cpp *.h)");
    windowHash.key(textedit)->setText(filename);
    textedit->setWindowTitle(filename);
    QFile *file = new QFile(filename);
    file->open(QIODevice::WriteOnly|QIODevice::Text);
    QFileInfo fileInfo(filename);
    if(fileInfo.isWritable()) {
        QByteArray msg = textedit->toHtml( ).toUtf8( );   // QString을 QByteArray로 변환
        file->write(msg);
    }
    else {
        QMessageBox::warning(this, "Error", "Can't Save this file", QMessageBox::Ok);
    }
    file->close();
    delete file;
    qDebug() << filename;
}

void QtEditor::printt()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage(true);
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec()==QDialog::Accepted) {
        QMdiSubWindow *subwindow = mdiArea->currentSubWindow();
        QTextEdit *textedit = dynamic_cast<QTextEdit*>(subwindow->widget());
        textedit->print(&printer);
    }
}

void QtEditor::selectWindow()
{
    QTextEdit* textedit = (QTextEdit*)windowHash[(QAction*)sender()];
    textedit->setFocus();
}

void QtEditor::closeWindow()
{
    QTextEdit *textedit = dynamic_cast<QTextEdit*>(sender());
    windowMenu->removeAction(windowHash.key(textedit));
    delete windowHash.key(textedit);
}

void QtEditor::setColor()
{
    QMdiSubWindow *subwindow = mdiArea->currentSubWindow();
    QTextEdit *textedit = dynamic_cast<QTextEdit*>(subwindow->widget());
    QColor color = QColorDialog::getColor(textedit->textColor(), this);
    if(color.isValid()) textedit->setTextColor(color);
}

void QtEditor::setFontt()
{
    bool ok;
    QMdiSubWindow * subwindow = mdiArea->currentSubWindow();
    QTextEdit * textedit = dynamic_cast<QTextEdit*>(subwindow->widget());
    QFont font = QFontDialog::getFont(&ok, textedit->currentFont(), this);
    if(ok) textedit->setCurrentFont(font);
}

void QtEditor::setFontWidget() {
    QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
    QTextEdit *textedit = dynamic_cast<QTextEdit*>(subWindow->widget());
    QFont font = textedit->currentFont();
}

void QtEditor::setTextFont(QFont fontBox) {    // 글자 종류 바꾸는 툴바 슬롯
    QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
    QTextEdit *textedit = dynamic_cast<QTextEdit*>(subWindow->widget());
    textedit->setCurrentFont(fontBox);
}

void QtEditor::setTextSize(qreal sizeBox) {    // 글자 크기 바꾸는 툴바 슬롯
    QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
    QTextEdit *textedit = dynamic_cast<QTextEdit*>(subWindow->widget());
    QFont font = textedit->currentFont();
    font.setPointSizeF(sizeBox);
    textedit->setCurrentFont(font);
}

void QtEditor::about()      // help menu : about action에 대한 슬롯
{
    QMessageBox::question(this, "QtEditor", "Copyright 3.4. @TaeHun Kim",         // 창 타이틀, 창 내용
                          QMessageBox::YesRole/*|QMessageBox::No|QMessageBox::Cancel*/);   // yes, no, cancel 버튼
}

#if 1
QAction *QtEditor::makeAction(QString icon, QString name, QString shortCut,         //icon 사용
                              QString toolTip, QObject* recv, const char* slot)
{
    QAction *act = new QAction(name, this);
    if(icon.length())                   // 파일이 있는지 없는지 검사(""을 입력하면 icon 없이 사용 가능)
        act->setIcon(QIcon(icon));      // act->setIcon(QIcon(":/image/" + icon)); -> image file 경로 설정 가능
    act->setShortcut(shortCut);
    act->setStatusTip(toolTip);

    connect(act, SIGNAL(triggered()), recv, slot);
    return act;
}

QAction *QtEditor::makeAction(QString name, QString shortCut,                       //icon 사용하지 않음
                              QString toolTip, QObject* recv, const char* slot)
{
    QAction *act = new QAction(name, this);
    act->setShortcut(shortCut);
    act->setStatusTip(toolTip);
    connect(act, SIGNAL(triggered()), recv, slot);
    return act;
}

QAction *QtEditor::makeAction(QString icon, QString name, QString shortCut, QString toolTip)
{                                                                   // edit action 사용자 정의 슬롯 사용
    QAction *act = new QAction(name, this);
    if(icon.length())
        act->setIcon(QIcon(icon));
    act->setShortcut(shortCut);
    act->setStatusTip(toolTip);
#if 0
    connect(act, &QAction::triggered, this, [=](){                              // lambda 활용
        QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
        QTextEdit *textedit = dynamic_cast<QTextEdit*>(subWindow->widget());

        if (toolTip=="Undo")            // toolTip이 Undo면 undo() 실행
            textedit->undo();
        else if (toolTip=="Redo")
            textedit->redo();
        else if (toolTip=="Copy")
            textedit->copy();
        else if (toolTip=="Cut")
            textedit->cut();
        else if (toolTip=="Paste")
            textedit->paste();
        else if (toolTip=="ZoomIn")
            textedit->zoomIn();
        else if (toolTip=="ZoomOut")
            textedit->zoomOut();

    });
#endif
    return act;
}

#else
template <typename T>
QAction *QtEditor::makeAction(QString icon, QString text,           // 탬플릿을 활용해서 action 구현하는 함수
                              T shortCut, QString toolTip,          // setShortcut에는 QString만 인자로 받을 수 있어서
                              QObject* recv, const char* slot)      // 템플릿이더라도 const char*을 인자로 받으면 컴파일 에러가 뜸
{                                                                   // 그래서 const char* 가 인자로 들어오면 QString으로
    QAction *act = new QAction(text, this);                         // 바꿔주는 코딩이 따로 필요함 -> 템플릿 특수화 필요
    if(icon.length( ))
        act->setIcon(QIcon(icon));
    act->setShortcut(shortCut);
    act->setStatusTip(toolTip);

    connect(act, SIGNAL(triggered( )), recv, slot);
    return act;
}

template <>     // 템플릿 특수화 : const char* shortCut가 들어올 때는 일반화된 템플릿에서 처리를 못하기 때문에 특수화를 통해 Qstring으로 형변환해준다.
QAction *QtEditor::makeAction(QString icon, QString text,
                              const char* shortCut, QString toolTip,
                              QObject* recv, const char* slot)
{
    QAction *act = new QAction(text, this);
    if(icon.length( ))
        act->setIcon(QIcon(icon));
    act->setShortcut(QString(shortCut));
    act->setStatusTip(toolTip);
    connect(act, SIGNAL(triggered( )), recv, slot);
    return act;
}

template <typename T, typename Functor>
QAction* QtEditor::makeAction(QString icon, QString text, T shortCut,
                              QString toolTip, Functor lambda)
{
    QAction *act = new QAction(text, this);
    if(icon.length( ))
         act->setIcon(QIcon(icon));
    act->setShortcut(shortCut);
    act->setStatusTip(toolTip);
    act->setToolTip(toolTip);
    connect(act, &QAction::triggered, this, lambda);
    return act;
}
#endif


