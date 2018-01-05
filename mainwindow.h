#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkAccessManager>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void my_slots(QNetworkReply*);

private:
    int i,j;
    QString str;    //聊天文件内容
    QString str1;   //对话表示
    QString str2;   //段落ID
    QString str3;   //段落内容
    QString string1;    //完整语句
    QStringList list1;
    QStringList list2;
    QFile *file;        //读取文件
    QFile *file1;       //写入文件
    QNetworkAccessManager *manager;
    QNetworkRequest req;
    void fun1();        //段落切割
    void fun2();        //对话切割
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
