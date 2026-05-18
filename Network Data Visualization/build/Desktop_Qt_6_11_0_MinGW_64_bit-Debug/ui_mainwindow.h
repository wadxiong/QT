/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editIP;
    QLabel *label_2;
    QLineEdit *editPort;
    QPushButton *btnConnect;
    QPushButton *btnDisconnect;
    QLabel *lblStatus;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnStart;
    QPushButton *btnPause;
    QPushButton *btnClear;
    QPushButton *btnExport;
    QLabel *label_3;
    QComboBox *cboInterval;
    QLabel *lblIndicator;
    QLabel *lblDataStatus;
    QHBoxLayout *horizontalLayout_3;
    QChartView *chartView;
    QTableWidget *tableWidget;
    QTextEdit *txtLog;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(900, 650);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        editIP = new QLineEdit(groupBox);
        editIP->setObjectName("editIP");

        horizontalLayout->addWidget(editIP);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        editPort = new QLineEdit(groupBox);
        editPort->setObjectName("editPort");

        horizontalLayout->addWidget(editPort);

        btnConnect = new QPushButton(groupBox);
        btnConnect->setObjectName("btnConnect");

        horizontalLayout->addWidget(btnConnect);

        btnDisconnect = new QPushButton(groupBox);
        btnDisconnect->setObjectName("btnDisconnect");

        horizontalLayout->addWidget(btnDisconnect);

        lblStatus = new QLabel(groupBox);
        lblStatus->setObjectName("lblStatus");

        horizontalLayout->addWidget(lblStatus);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        btnStart = new QPushButton(groupBox_2);
        btnStart->setObjectName("btnStart");

        horizontalLayout_2->addWidget(btnStart);

        btnPause = new QPushButton(groupBox_2);
        btnPause->setObjectName("btnPause");

        horizontalLayout_2->addWidget(btnPause);

        btnClear = new QPushButton(groupBox_2);
        btnClear->setObjectName("btnClear");

        horizontalLayout_2->addWidget(btnClear);

        btnExport = new QPushButton(groupBox_2);
        btnExport->setObjectName("btnExport");

        horizontalLayout_2->addWidget(btnExport);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName("label_3");

        horizontalLayout_2->addWidget(label_3);

        cboInterval = new QComboBox(groupBox_2);
        cboInterval->setObjectName("cboInterval");

        horizontalLayout_2->addWidget(cboInterval);

        lblIndicator = new QLabel(groupBox_2);
        lblIndicator->setObjectName("lblIndicator");
        lblIndicator->setMinimumSize(QSize(20, 20));

        horizontalLayout_2->addWidget(lblIndicator);

        lblDataStatus = new QLabel(groupBox_2);
        lblDataStatus->setObjectName("lblDataStatus");

        horizontalLayout_2->addWidget(lblDataStatus);


        verticalLayout_2->addWidget(groupBox_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        chartView = new QChartView(centralwidget);
        chartView->setObjectName("chartView");
        chartView->setMinimumSize(QSize(500, 300));

        horizontalLayout_3->addWidget(chartView);

        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setMinimumSize(QSize(300, 300));

        horizontalLayout_3->addWidget(tableWidget);


        verticalLayout_2->addLayout(horizontalLayout_3);

        txtLog = new QTextEdit(centralwidget);
        txtLog->setObjectName("txtLog");
        txtLog->setMaximumSize(QSize(16777215, 100));

        verticalLayout_2->addWidget(txtLog);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 900, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\347\275\221\347\273\234\350\277\236\346\216\245\351\205\215\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\346\234\215\345\212\241\345\231\250IP\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\347\253\257\345\217\243\357\274\232", nullptr));
        btnConnect->setText(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245\346\234\215\345\212\241\345\231\250", nullptr));
        btnDisconnect->setText(QCoreApplication::translate("MainWindow", "\346\226\255\345\274\200\350\277\236\346\216\245", nullptr));
        lblStatus->setText(QCoreApplication::translate("MainWindow", "\346\234\252\350\277\236\346\216\245", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\346\216\247\345\210\266", nullptr));
        btnStart->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\346\216\245\346\224\266", nullptr));
        btnPause->setText(QCoreApplication::translate("MainWindow", "\346\232\202\345\201\234\346\216\245\346\224\266", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\346\225\260\346\215\256", nullptr));
        btnExport->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272CSV", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\345\210\267\346\226\260\351\242\221\347\216\207\357\274\232", nullptr));
        lblIndicator->setText(QString());
        lblDataStatus->setText(QCoreApplication::translate("MainWindow", "\346\234\252\347\233\221\346\265\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
