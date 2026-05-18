#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateEdit>
#include <QSpinBox>
#include <QMessageBox>
#include "bookmanager.h"
#include "readermanager.h"
#include "borrowmanager.h"

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
    // 图书管理
    void onAddBook();
    void onEditBook();
    void onDeleteBook();
    void onSearchBook();
    void onBookTableSelectionChanged();
    
    // 读者管理
    void onAddReader();
    void onEditReader();
    void onDeleteReader();
    void onReaderTableSelectionChanged();
    
    // 借阅管理
    void onBorrowBook();
    void onReturnBook();
    void onRefreshBorrowList();
    
    // 标签切换
    void onTabChanged(int index);

private:
    void setupUI();
    void setupConnections();
    void setupTableStyles();
    void loadAllData();
    void saveAllData();
    void refreshBookTable(const QList<Book> &books);
    void refreshReaderTable();
    void refreshBorrowTable();
    void clearBookForm();
    void clearReaderForm();
    void updateStatusBar();
    
    Ui::MainWindow *ui;
    
    // 数据管理器
    BookManager m_bookManager;
    ReaderManager m_readerManager;
    BorrowManager m_borrowManager;
    
    // 文件路径
    const QString BOOK_FILE = "books.txt";
    const QString READER_FILE = "readers.txt";
    const QString BORROW_FILE = "borrows.txt";
    
    // 当前选中的行
    int m_selectedBookRow;
    int m_selectedReaderRow;
};
#endif // MAINWINDOW_H