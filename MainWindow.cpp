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

    //network config
    url = QUrl("https://rate.bot.com.tw/xrt/flcsv/0/day");
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onRequestFinished(QNetworkReply *)));

    //ui config
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->label_Source->setOpenExternalLinks(true);
    ui->label_Source->setText(tr("<a href=\"https://rate.bot.com.tw/xrt?Lang=zh-TW\">台灣銀行牌告匯率</a>"));
    ui->label_CSVSource->setOpenExternalLinks(true);
    ui->label_CSVSource->setText(tr("<a href=\"https://rate.bot.com.tw/xrt/flcsv/0/day\">台灣銀行當日匯率CSV下載網址</a>"));

    connect(ui->btn_Request, SIGNAL(clicked()), this, SLOT(onSearch()));
    //connect(ui->btn_download, SIGNAL(clicked()), this, SLOT(onDownloadData()));
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

void MainWindow::onSearch()
{
    QNetworkRequest request(url);
    manager->get(request);
}

void MainWindow::onUpdateTableWidget(const QList<QStringList> &list)
{
    ui->tableWidget->setRowCount(list.size()-1); //header不能算
    for(int i=1; i<list.size(); i++)
    {
        QTableWidgetItem *itemCurrency = new QTableWidgetItem(list.at(i).at(0));
        QTableWidgetItem *itemDescription = new QTableWidgetItem(list.at(i).at(1));
        QTableWidgetItem *itemBuy = new QTableWidgetItem(list.at(i).at(2));
        QTableWidgetItem *itemSell = new QTableWidgetItem(list.at(i).at(3));

        ui->tableWidget->setItem(i-1, 0, itemCurrency);
        ui->tableWidget->setItem(i-1, 1, itemDescription);
        ui->tableWidget->setItem(i-1, 2, itemBuy);
        ui->tableWidget->setItem(i-1, 3, itemSell);
    }
}

void MainWindow::onRequestFinished(QNetworkReply *reply)
{
    qDebug() << "request finish!";
    reply->setTextModeEnabled(true);

    //讀取url的csv檔案
    QList<QStringList> retList = ReadFromCSV(reply);

    //update table widget
    onUpdateTableWidget(retList);

//    for(int i=0; i<retList.size(); i++)
//    {
//        qDebug()<<retList.at(i);
//        qDebug()<<"";
//    }
}
