#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QList>
#include <QStringList>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onDownloadFinished(QNetworkReply *)));

    QNetworkRequest request(QUrl("https://rate.bot.com.tw/xrt/flcsv/0/day"));
    manager->get(request);

}

MainWindow::~MainWindow()
{
    delete ui;
}

QList<QStringList> MainWindow::ReadFromCSV(QNetworkReply *reply)
{
    QList<QStringList> listRes;
    while(!reply->atEnd()){
        QString line = reply->readLine();
        line.replace('\n',"");
        QStringList lineList = line.split(',');
        listRes.append(lineList);
    }

    return listRes;
}

void MainWindow::onDownloadFinished(QNetworkReply *reply)
{
    qDebug() << "request finish!";
    reply->setTextModeEnabled(true);

    QList<QStringList> retList = ReadFromCSV(reply);

    for(int i=0; i<retList.size(); i++)
    {
        qDebug()<<retList.at(i);
        qDebug()<<"";
    }

}
