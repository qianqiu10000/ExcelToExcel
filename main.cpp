#include "MainWindow.h"

#include<QFile>
#include <QApplication>

QFile logFile("log.txt");
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(!logFile.isOpen())//如果日志打开失败，直接返回
        if(!logFile.open(QFile::WriteOnly|QFile::Append))
            return;

    //获取格式化的消息
    QString text=qFormatLogMessage(type,context,msg);

    //保存消息到日志
    text.append('\n');
    logFile.write(text.toLocal8Bit());
    logFile.flush();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//   qSetMessagePattern("[%{time hh:mm:ss}] [%{file}_%{line}]:%{message}");
//    qInstallMessageHandler(myMessageOutput);

    MainWindow widget;

    widget.show();

    int r=a.exec();
    logFile.close();
    return r;
}
