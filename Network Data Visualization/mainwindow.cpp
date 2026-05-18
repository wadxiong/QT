#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QStringConverter> // 新增头文件

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_isConnected(false)
    , m_isReceiving(false)
    , m_csvStream(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("网络数据可视化客户端");
    setMinimumSize(900, 650);

    initStyle();
    initUI();
    initChart();
    initCsvFile();

    m_tcpSocket = new QTcpSocket(this);
    connect(m_tcpSocket, &QTcpSocket::connected, this, &MainWindow::onSocketConnected);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &MainWindow::onSocketDisconnected);
    connect(m_tcpSocket, &QTcpSocket::errorOccurred, this, &MainWindow::onSocketError);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::onSocketReadyRead);

    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setInterval(3000);
    connect(m_reconnectTimer, &QTimer::timeout, this, &MainWindow::reconnectToServer);
}

MainWindow::~MainWindow()
{
    if(m_csvStream) delete m_csvStream;
    if(m_csvFile.isOpen()) m_csvFile.close();
    if(m_tcpSocket->isOpen()) m_tcpSocket->close();
    delete ui;
}

void MainWindow::initStyle()
{
    this->setStyleSheet(R"(
        QMainWindow {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #f0f8ff, stop:1 #d6eaff);
        }
        QGroupBox {
            font-size:13px;
            font-weight:bold;
            border:1px solid #cccccc;
            border-radius:6px;
            margin-top:6px;
            padding-top:6px;
        }
        QPushButton {
            background-color:#409eff;
            color:white;
            border:none;
            border-radius:5px;
            padding:6px 12px;
            font-size:12px;
        }
        QPushButton:hover {
            background-color:#66b1ff;
        }
        QPushButton:disabled {
            background-color:#c0c4cc;
        }
        QLineEdit, QComboBox {
            border:1px solid #dcdfe6;
            border-radius:4px;
            padding:4px;
            font-size:12px;
        }
        QTableWidget {
            border:1px solid #dcdfe6;
            gridline-color:#e4e7ed;
            font-size:12px;
        }
        QHeaderView::section {
            background-color:#f2f6fc;
            padding:4px;
            border:none;
        }
        QTextEdit {
            border:1px solid #dcdfe6;
            border-radius:4px;
            font-size:12px;
        }
        QLabel {
            font-size:12px;
        }
    )");
    ui->lblIndicator->setStyleSheet("background-color:gray; border-radius:10px;");
    ui->lblIndicator->setFixedSize(20,20);
}

void MainWindow::initUI() {
    ui->editIP->setText("127.0.0.1");
    ui->editPort->setText("8080");
    ui->cboInterval->addItems({"0.5s","1s","2s","3s","4s","5s"});
    ui->cboInterval->setCurrentIndex(1);

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"时间","温度(℃)","湿度(%)","状态"});
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->btnDisconnect->setEnabled(false);
    ui->btnStart->setEnabled(false);
    ui->btnPause->setEnabled(false);
}

void MainWindow::initChart() {
    m_seriesTemp = new QLineSeries();
    m_seriesHumi = new QLineSeries();
    m_seriesTemp->setName("温度");
    m_seriesHumi->setName("湿度");
    m_seriesTemp->setColor("#ff4d4f");
    m_seriesHumi->setColor("#36cbcb");

    m_chart = new QChart();
    m_chart->addSeries(m_seriesTemp);
    m_chart->addSeries(m_seriesHumi);
    m_chart->setTitle("实时环境监测数据");
    m_chart->setBackgroundBrush(QColor(255,255,255,220));
    m_chart->setPlotAreaBackgroundBrush(QColor(248,250,252));
    m_chart->legend()->setVisible(true);

    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    m_axisX->setTitleText("数据序列");
    m_axisY->setTitleText("数值范围");
    m_axisY->setRange(0, 100);

    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_seriesTemp->attachAxis(m_axisX);
    m_seriesTemp->attachAxis(m_axisY);
    m_seriesHumi->attachAxis(m_axisX);
    m_seriesHumi->attachAxis(m_axisY);

    ui->chartView->setChart(m_chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::initCsvFile() {
    QString date = QDateTime::currentDateTime().toString("yyyyMMdd");
    m_csvFileName = QString("data_%1.csv").arg(date);
    m_csvFile.setFileName(m_csvFileName);

    if(m_csvFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        m_csvStream = new QTextStream(&m_csvFile);
        // 修复：Qt6 用 setEncoding 替代 setCodec
        m_csvStream->setEncoding(QStringConverter::Utf8);
        if(m_csvFile.size() == 0) {
            *m_csvStream << "时间,温度,湿度,状态\n";
        }
    }
}

void MainWindow::on_btnConnect_clicked() {
    ui->lblStatus->setText("🔄 连接中...");
    QString ip = ui->editIP->text();
    quint16 port = ui->editPort->text().toUInt();
    m_tcpSocket->connectToHost(ip, port);
}

void MainWindow::on_btnDisconnect_clicked() {
    m_reconnectTimer->stop();
    m_tcpSocket->close();
}

void MainWindow::onSocketConnected() {
    m_isConnected = true;
    ui->lblStatus->setText("✅ 已连接");
    ui->btnConnect->setEnabled(false);
    ui->btnDisconnect->setEnabled(true);
    ui->btnStart->setEnabled(true);
    m_reconnectTimer->stop();
}

void MainWindow::onSocketDisconnected() {
    m_isConnected = false;
    m_isReceiving = false;
    ui->lblStatus->setText("❌ 已断开");
    ui->btnConnect->setEnabled(true);
    ui->btnDisconnect->setEnabled(false);
    ui->btnStart->setEnabled(false);
    ui->btnPause->setEnabled(false);
    m_reconnectTimer->start();
}

void MainWindow::onSocketError(QAbstractSocket::SocketError) {
    ui->lblStatus->setText("⚠️ 连接错误：" + m_tcpSocket->errorString());
}

void MainWindow::reconnectToServer() {
    if(!m_isConnected) {
        m_tcpSocket->connectToHost(ui->editIP->text(), ui->editPort->text().toUInt());
    }
}

void MainWindow::onSocketReadyRead() {
    if(!m_isReceiving) return;
    while(m_tcpSocket->canReadLine()) {
        QByteArray data = m_tcpSocket->readLine().trimmed();
        if(!data.isEmpty()) parseJsonData(data);
    }
}

void MainWindow::parseJsonData(const QByteArray &data) {
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if(err.error != QJsonParseError::NoError) {
        ui->txtLog->append("❌ JSON解析失败");
        return;
    }

    QJsonObject obj = doc.object();
    QVariantMap map;
    map["time"] = QDateTime::fromSecsSinceEpoch(obj["timestamp"].toVariant().toLongLong()).toString("HH:mm:ss");
    map["temp"] = obj["value1"].toDouble();
    map["humi"] = obj["value2"].toDouble();
    map["status"] = obj["status"].toString();

    m_dataList.append(map);
    writeDataToCsv(map);
    updateUI(map);
}

void MainWindow::updateUI(const QVariantMap &data) {
    ui->txtLog->append(QString("[%1] 温度：%2 ℃ | 湿度：%3 %RH")
                       .arg(data["time"].toString())
                       .arg(data["temp"].toDouble(), 0, 'f', 1)
                       .arg(data["humi"].toDouble(), 0, 'f', 1));

    setStatusIndicator(data["status"].toString());

    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem(data["time"].toString()));
    ui->tableWidget->setItem(0,1,new QTableWidgetItem(QString::number(data["temp"].toDouble(),'f',1)));
    ui->tableWidget->setItem(0,2,new QTableWidgetItem(QString::number(data["humi"].toDouble(),'f',1)));
    ui->tableWidget->setItem(0,3,new QTableWidgetItem(data["status"].toString()));

    if(ui->tableWidget->rowCount() > MAX_TABLE_ROWS) ui->tableWidget->removeRow(MAX_TABLE_ROWS);

    static int idx = 0;
    m_seriesTemp->append(idx, data["temp"].toDouble());
    m_seriesHumi->append(idx, data["humi"].toDouble());
    idx++;

    if(m_seriesTemp->count() > MAX_CHART_POINTS) {
        m_seriesTemp->remove(0);
        m_seriesHumi->remove(0);
    }
    m_axisX->setRange(0, m_seriesTemp->count());
}

void MainWindow::setStatusIndicator(const QString &status) {
    if(status == "normal") {
        ui->lblIndicator->setStyleSheet("background-color:#00b42a; border-radius:10px;");
        ui->lblDataStatus->setText("正常");
    } else if(status == "warning") {
        ui->lblIndicator->setStyleSheet("background-color:#ff7d00; border-radius:10px;");
        ui->lblDataStatus->setText("警告");
    } else {
        ui->lblIndicator->setStyleSheet("background-color:#f53f3f; border-radius:10px;");
        ui->lblDataStatus->setText("异常");
    }
}

void MainWindow::writeDataToCsv(const QVariantMap &data) {
    if(m_csvStream) {
        *m_csvStream << data["time"].toString() << ","
                    << data["temp"].toDouble() << ","
                    << data["humi"].toDouble() << ","
                    << data["status"].toString() << "\n";
        m_csvStream->flush();
    }
}

void MainWindow::on_btnExport_clicked() {
    QString path = QFileDialog::getSaveFileName(this,"导出CSV","","CSV(*.csv)");
    if(path.isEmpty()) return;
    if(QFile::copy(m_csvFileName, path)) {
        QMessageBox::information(this,"成功","导出完成！");
    } else {
        QMessageBox::warning(this,"失败","导出失败！");
    }
}

void MainWindow::on_btnStart_clicked() {
    m_isReceiving = true;
    ui->btnStart->setEnabled(false);
    ui->btnPause->setEnabled(true);
}

void MainWindow::on_btnPause_clicked() {
    m_isReceiving = false;
    ui->btnStart->setEnabled(true);
    ui->btnPause->setEnabled(false);
}

void MainWindow::on_btnClear_clicked() {
    m_seriesTemp->clear();
    m_seriesHumi->clear();
    ui->tableWidget->setRowCount(0);
    ui->txtLog->clear();
    m_dataList.clear();
}

void MainWindow::on_cboInterval_currentIndexChanged(int) {}