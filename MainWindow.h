#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

#include "CurrencyRateManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QUrl url;
    QNetworkAccessManager *manager;
    CurrencyRateManager crManager;

private:
    QList<QStringList> ReadFromCSV(QNetworkReply *reply);
    void Add2CurrencyRateManager(const QList<QStringList> &list);


public slots:
    void onSearch();
    void onUpdateTableWidget();
    void onUpdateDownloadCombo();
    void onRequestFinished(QNetworkReply *reply);
};

#endif // MAINWINDOW_H
