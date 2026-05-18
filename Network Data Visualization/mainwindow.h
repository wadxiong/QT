#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QTimer>
#include <QFile>
#include <QVariantMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnConnect_clicked();
    void on_btnDisconnect_clicked();
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError(QAbstractSocket::SocketError error);
    void onSocketReadyRead();
    void reconnectToServer();

    void on_btnStart_clicked();
    void on_btnPause_clicked();
    void on_btnClear_clicked();
    void on_btnExport_clicked();
    void on_cboInterval_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    QTcpSocket *m_tcpSocket;
    QTimer *m_reconnectTimer;
    bool m_isConnected;

    bool m_isReceiving;
    QList<QVariantMap> m_dataList;
    const int MAX_TABLE_ROWS = 20;
    const int MAX_CHART_POINTS = 60;

    QChart *m_chart;
    QLineSeries *m_seriesTemp;
    QLineSeries *m_seriesHumi;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;

    QFile m_csvFile;
    QString m_csvFileName;
    QTextStream *m_csvStream;

    void initStyle();
    void initUI();
    void initChart();
    void initCsvFile();
    void parseJsonData(const QByteArray &data);
    void updateUI(const QVariantMap &data);
    void setStatusIndicator(const QString &status);
    void writeDataToCsv(const QVariantMap &data);
};
#endif // MAINWINDOW_H