#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include "StudentManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addBtnClicked();
    void updateBtnClicked();
    void deleteBtnClicked();
    void clearBtnClicked();
    void searchBtnClicked();
    void statBtnClicked();
    void sortAscBtnClicked();
    void sortDescBtnClicked();
    void tableCellClicked(int row, int col);
    void showAllData();

private:
    void initUI();
    void setUIStyle();
    void refreshTable(const QList<Student>& list = {});
    bool checkInputEmpty();

    QLineEdit *edtId, *edtName, *edtClass, *edtCourse;
    QDoubleSpinBox *spinScore;
    QPushButton *btnAdd, *btnUpdate, *btnDelete, *btnClear;
    QLineEdit *edtSearch, *edtStatCourse;
    QComboBox *cboSearchType;
    QPushButton *btnSearch, *btnStat, *btnSortAsc, *btnSortDesc,*btnShowAll;
    QLabel *labStatResult;
    QTableWidget *tableWidget;
    StudentManager* m_manager;
};

#endif // MAINWINDOW_H