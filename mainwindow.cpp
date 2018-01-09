#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    over=0;
    i=1;
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(my_slots(QNetworkReply*)));
    req.setUrl(QUrl("https://aip.baidubce.com/rpc/2.0/nlp/v1/sentiment_classify?access_token=24.c31c6a1a9384089690f49673bc8b37c9.2592000.1517642051.282335-10624746"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::my_slots(QNetworkReply *reply){
        QByteArray bytes = reply->readAll();
        QString string = QString::fromLocal8Bit(bytes);
        string=string.right(4).left(1);
        QString ss;
        if(string=="0"){
            ss="消极";
        }else if(string == "1"){
            ss="平常";
        }else if(string == "2"){
            ss="积极";
        }
        string1=QString("[- %1 -]").arg(ss) + string1;
        QByteArray ba=string1.toUtf8();
        qDebug()<<file1->write(ba);
        fun2();
        reply->deleteLater();
}


void MainWindow::on_pushButton_clicked()
{
    QString str(ui->lineEdit->text());
    file= new QFile(str);
    file1 = new QFile("c:/tmp/dialogue.txt");
    if(!file->open(QFile::ReadOnly | QFile::Text)) {
            qDebug()<<"Can't open the file!"<<endl;
    }
    if(!file1->open(QFile::WriteOnly | QFile::Append)) {
           qDebug()<<"Can't open the file1!"<<endl;
    }
    txtInput=new QTextStream(file);
    this->fun1();
}

void MainWindow::fun(){
    QByteArray lineStr;
    lineStr=file->read(4096);
    if(list1.size()%6==1 || i==1){
        str = QString::fromLocal8Bit(lineStr);
    }else{
        str = QString("\"%1%2").arg(str.section("\"\"",-1,-1)).arg(QString::fromLocal8Bit(lineStr));
    }
    qDebug()<<str;
    list1 = str.split('"');
    i=1;
    if(txtInput->atEnd()){
        qDebug()<<"---------------------------------";
        over=1;
    }
    return;
}
void MainWindow::fun1(){
    if(over==1){
        return;
    }
    if(list1.size()-i<6){
        fun();
        qDebug()<<list1.size()<<"   i="<<i;
    }
    for(int m=0;m<3;m++){
        str1[m] = list1.at(i);
        i+=2;
    }
    list2 = str1[2].split(':');
    j=0;
    this->fun2();
}

void MainWindow::fun2(){
    if(j>=list2.size()-1){
        this->fun1();
        return;
    }
    QString ss1=QString(list2.at(j++)).section(';',-1);
    QString ss2=QString(list2.at(j));
    ss2.chop(QString(list2.at(j)).section(';',-1).size());

    QString string=QString("{\"text\":\"%1\"}").arg(ss2);
    QByteArray ba1=string.toLocal8Bit();
    manager->post(req, ba1);
    string1=QString("%1     %2      %3      %4     %5\r\n").arg(str1[0]).arg(str1[1]).arg(j).arg(ss1).arg(ss2);
}
