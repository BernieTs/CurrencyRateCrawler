#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QList>
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>

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
    ui->lineEdit_filePath->setText(QApplication::applicationDirPath());

    connect(ui->btn_Request, SIGNAL(clicked()), this, SLOT(onSearch()));
    connect(ui->comboBox_currency, SIGNAL(currentTextChanged(QString)), this, SLOT(onComboBoxTextChanged(QString)));
    connect(ui->btn_download, SIGNAL(clicked()), this, SLOT(onDownloadData()));
    connect(ui->btn_ChangeDownloadDir, SIGNAL(clicked()), this, SLOT(onChangeDownloadDir()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::ReadFromCSV
 * 將QNetWorkReply擷取到的CSV資料轉換成QList形式
 * @param reply 擷取到的CSV資料
 * @return 轉換過後的CSV資料，以QList形式輸出
 */
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

/**
 * @brief MainWindow::Add2CurrencyRateManager
 * 將CSV資料記錄到CurrencyRateManager，方便後續查找與下載用
 * @param list
 */
void MainWindow::Add2CurrencyRateManager(const QList<QStringList> &list)
{
    crManager.RemoveAllCurrencyRate();

    for(int i=1; i<list.size(); i++)
    {
        CurrencyRate cr;
        cr.CurrName = QString(list.at(i).at(0)).toStdString();
        cr.sDescription = QString(list.at(i).at(1)).toStdString();
        cr.buyValue = QString(list.at(i).at(2)).toFloat();
        cr.sellValue = QString(list.at(i).at(3)).toFloat();

        crManager.AddCurrencyRate(cr);
    }
}

void MainWindow::onSearch()
{
    QNetworkRequest request(url);
    manager->get(request);
}

void MainWindow::onUpdateTableWidget()
{
    ui->tableWidget->clearContents();
    std::vector<CurrencyRate> rates = crManager.GetAllCurrencyRateInfo();
    ui->tableWidget->setRowCount(rates.size());
    for(int i=0; i<rates.size(); i++)
    {
        CurrencyRate cr = rates.at(i);
        QTableWidgetItem *itemCurrency = new QTableWidgetItem(QString::fromStdString(cr.CurrName));
        QTableWidgetItem *itemDescription = new QTableWidgetItem(QString::fromStdString(cr.sDescription));
        QTableWidgetItem *itemBuy = new QTableWidgetItem(QString::number(cr.buyValue));
        QTableWidgetItem *itemSell = new QTableWidgetItem(QString::number(cr.sellValue));

        ui->tableWidget->setItem(i, 0, itemCurrency);
        ui->tableWidget->setItem(i, 1, itemDescription);
        ui->tableWidget->setItem(i, 2, itemBuy);
        ui->tableWidget->setItem(i, 3, itemSell);
    }
}

void MainWindow::onUpdateDownloadCombo()
{
    ui->comboBox_currency->clear();

    std::vector<CurrencyRate> rates = crManager.GetAllCurrencyRateInfo();
    for(int i=0; i<rates.size(); i++)
        ui->comboBox_currency->addItem(QString::fromStdString(rates.at(i).CurrName));
}

void MainWindow::onDownloadData()
{
    QString filename = ui->lineEdit_fileName->text() + ".txt";
    QFileInfo fileInfo(ui->lineEdit_filePath->text(), filename);

    std::string sCurrencyType = ui->comboBox_currency->currentText().toStdString();
    if(!crManager.DownloadCurrencyRateByName(sCurrencyType, fileInfo.absoluteFilePath()))
    {
        QMessageBox::warning(this, "下載錯誤", "下載資料錯誤，請重新操作");
        return;
    }

    if(ui->checkBox_LauchFile->isChecked())
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
}

void MainWindow::onChangeDownloadDir()
{
    QString qDir = QFileDialog::getExistingDirectory(this, "下載位置",
                                                     QApplication::applicationDirPath(),
                                                     QFileDialog::ShowDirsOnly);
    ui->lineEdit_filePath->setText(qDir);
}

void MainWindow::onComboBoxTextChanged(QString s)
{
    QString fileName = sCurrentDateTime + "_" + s;
    ui->lineEdit_fileName->setText(fileName);
}

void MainWindow::onRequestFinished(QNetworkReply *reply)
{
    qDebug() << "request finish!";
    reply->setTextModeEnabled(true);

    //紀錄搜尋時間
    sCurrentDateTime = QDateTime::currentDateTime().toString("yyyyMMdd-hh-mm-ss");

    //讀取url的csv檔案
    QList<QStringList> retList = ReadFromCSV(reply);

    crManager.RemoveAllCurrencyRate();

    //csv檔寫入進CurrencyRate Mananger
    Add2CurrencyRateManager(retList);

    //update table widget
    onUpdateTableWidget();

    //update download combo box
    onUpdateDownloadCombo();
}
