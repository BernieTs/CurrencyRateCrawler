#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

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

private:
    QList<QStringList> ReadFromCSV(QNetworkReply *reply);

public slots:
    void onSearch();
    void onUpdateTableWidget(const QList<QStringList> &list);
    void onRequestFinished(QNetworkReply *reply);
};

#endif // MAINWINDOW_H
