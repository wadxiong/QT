#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 设置应用程序样式
    a.setStyle(QStyleFactory::create("Fusion"));
    
    // 设置应用程序信息
    QApplication::setApplicationName("图书管理系统");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("Library");
    
    // 设置全局样式表
    QString styleSheet = R"(
        QMainWindow {
            background-color: #f5f5f5;
        }
        QGroupBox {
            font-weight: bold;
            border: 1px solid #ddd;
            border-radius: 4px;
            margin-top: 10px;
            padding-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }
        QPushButton {
            padding: 6px 12px;
            border: 1px solid #ccc;
            border-radius: 4px;
            background-color: #fff;
        }
        QPushButton:hover {
            background-color: #e6e6e6;
        }
        QPushButton:pressed {
            background-color: #d4d4d4;
        }
        QLineEdit, QSpinBox, QDateEdit {
            padding: 6px;
            border: 1px solid #ddd;
            border-radius: 4px;
        }
        QLineEdit:focus, QSpinBox:focus, QDateEdit:focus {
            border-color: #0078d7;
        }
        QTabWidget::pane {
            border: 1px solid #ddd;
            border-radius: 4px;
            background-color: white;
        }
        QTabBar::tab {
            padding: 8px 16px;
            margin-right: 2px;
        }
        QTabBar::tab:selected {
            background-color: #0078d7;
            color: white;
        }
        QComboBox {
            padding: 6px;
            border: 1px solid #ddd;
            border-radius: 4px;
        }
    )";
    
    a.setStyleSheet(styleSheet);
    
    MainWindow w;
    w.show();
    
    return a.exec();
}