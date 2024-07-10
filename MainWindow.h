#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



class QAction;
class myPurchase;
class mySourceFile;
class myTableWidget;
class myMachine;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



protected:
   void dragEnterEvent(QDragEnterEvent *event)override;
   void dropEvent(QDropEvent *event)override;


private slots:
    void actionTriggered(QAction *action);

private:
    QAction*m_drawAction;
    QAction*m_eraseAction;
    QAction*m_createAction;
    QAction*m_clearAction;
    myTableWidget*m_table;
    QToolBar*m_toolBar;

    mySourceFile*m_source;
    myPurchase*m_purchase;
    myMachine*m_machine;

    void init();
    void createUI();
    void createSource(const QString&file);
    void createPurchase();
    void createMachine();
    void generateFile();


    void createConnection();
    void setSourceFile(const QString&file);//设置原文件
};
#endif // MAINWINDOW_H
