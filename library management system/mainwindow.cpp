#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QStatusBar>
#include <QComboBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_selectedBookRow(-1)
    , m_selectedReaderRow(-1)
{
    ui->setupUi(this);
    setupUI();
    setupConnections();
    setupTableStyles();
    loadAllData();
    refreshBookTable(m_bookManager.getAllBooks());
    refreshReaderTable();
    refreshBorrowTable();
    updateStatusBar();
}

MainWindow::~MainWindow()
{
    saveAllData();
    delete ui;
}

void MainWindow::setupUI()
{
    setWindowTitle("图书管理系统");
    resize(1200, 800);
    
    // 创建中心部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // 创建标签页
    QTabWidget *tabWidget = new QTabWidget();
    mainLayout->addWidget(tabWidget);
    
    // ============ 图书管理标签页 ============
    QWidget *bookTab = new QWidget();
    QHBoxLayout *bookLayout = new QHBoxLayout(bookTab);
    
    // 左侧 - 图书列表
    QVBoxLayout *bookLeftLayout = new QVBoxLayout();
    
    // 搜索区域
    QGroupBox *searchGroup = new QGroupBox("搜索图书");
    QHBoxLayout *searchLayout = new QHBoxLayout();
    QComboBox *searchType = new QComboBox();
    searchType->addItems({"书名", "作者", "ISBN"});
    QLineEdit *searchInput = new QLineEdit();
    searchInput->setPlaceholderText("输入搜索关键词...");
    QPushButton *searchBtn = new QPushButton("搜索");
    searchBtn->setIcon(QIcon::fromTheme("edit-find"));
    QPushButton *showAllBtn = new QPushButton("显示全部");
    
    searchLayout->addWidget(new QLabel("搜索方式:"));
    searchLayout->addWidget(searchType);
    searchLayout->addWidget(searchInput);
    searchLayout->addWidget(searchBtn);
    searchLayout->addWidget(showAllBtn);
    searchGroup->setLayout(searchLayout);

    searchBtn->setObjectName("searchBtn");
    showAllBtn->setObjectName("showAllBtn");

    QTableWidget *bookTable = new QTableWidget();
    bookTable->setColumnCount(5);
    bookTable->setHorizontalHeaderLabels({"书名", "作者", "ISBN", "出版年份", "库存"});
    bookTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    bookTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bookTable->setSelectionMode(QAbstractItemView::SingleSelection);
    bookTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bookTable->setAlternatingRowColors(true);
    
    bookLeftLayout->addWidget(searchGroup);
    bookLeftLayout->addWidget(bookTable);
    
    // 右侧 - 图书操作
    QVBoxLayout *bookRightLayout = new QVBoxLayout();
    QGroupBox *bookFormGroup = new QGroupBox("图书信息");
    QFormLayout *bookFormLayout = new QFormLayout();
    
    QLineEdit *bookTitleEdit = new QLineEdit();
    bookTitleEdit->setPlaceholderText("请输入书名");
    QLineEdit *bookAuthorEdit = new QLineEdit();
    bookAuthorEdit->setPlaceholderText("请输入作者");
    QLineEdit *bookIsbnEdit = new QLineEdit();
    bookIsbnEdit->setPlaceholderText("请输入ISBN号");
    QSpinBox *bookYearSpin = new QSpinBox();
    bookYearSpin->setRange(1900, 2100);
    bookYearSpin->setValue(2024);
    QSpinBox *bookStockSpin = new QSpinBox();
    bookStockSpin->setRange(0, 9999);
    bookStockSpin->setValue(1);
    
    bookFormLayout->addRow("书名:", bookTitleEdit);
    bookFormLayout->addRow("作者:", bookAuthorEdit);
    bookFormLayout->addRow("ISBN:", bookIsbnEdit);
    bookFormLayout->addRow("出版年份:", bookYearSpin);
    bookFormLayout->addRow("库存数量:", bookStockSpin);
    bookFormGroup->setLayout(bookFormLayout);
    
    QHBoxLayout *bookBtnLayout = new QHBoxLayout();
    QPushButton *addBookBtn = new QPushButton("添加");
    addBookBtn->setIcon(QIcon::fromTheme("list-add"));
    QPushButton *editBookBtn = new QPushButton("修改");
    editBookBtn->setIcon(QIcon::fromTheme("document-edit"));
    QPushButton *deleteBookBtn = new QPushButton("删除");
    deleteBookBtn->setIcon(QIcon::fromTheme("list-remove"));
    QPushButton *clearBookBtn = new QPushButton("清空");
    
    addBookBtn->setObjectName("addBookBtn");
    editBookBtn->setObjectName("editBookBtn");
    deleteBookBtn->setObjectName("deleteBookBtn");
    clearBookBtn->setObjectName("clearBookBtn");

    bookLayout->addLayout(bookLeftLayout, 3);
    bookLayout->addLayout(bookRightLayout, 2);
    
    // ============ 读者管理标签页 ============
    QWidget *readerTab = new QWidget();
    QHBoxLayout *readerLayout = new QHBoxLayout(readerTab);
    
    // 左侧 - 读者列表
    QTableWidget *readerTable = new QTableWidget();
    readerTable->setColumnCount(4);
    readerTable->setHorizontalHeaderLabels({"读者ID", "姓名", "电话", "邮箱"});
    readerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    readerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    readerTable->setSelectionMode(QAbstractItemView::SingleSelection);
    readerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    readerTable->setAlternatingRowColors(true);
    
    // 右侧 - 读者操作
    QVBoxLayout *readerRightLayout = new QVBoxLayout();
    QGroupBox *readerFormGroup = new QGroupBox("读者信息");
    QFormLayout *readerFormLayout = new QFormLayout();
    
    QLineEdit *readerIdEdit = new QLineEdit();
    readerIdEdit->setPlaceholderText("请输入读者ID（学号/工号）");
    QLineEdit *readerNameEdit = new QLineEdit();
    readerNameEdit->setPlaceholderText("请输入姓名");
    QLineEdit *readerPhoneEdit = new QLineEdit();
    readerPhoneEdit->setPlaceholderText("请输入电话");
    QLineEdit *readerEmailEdit = new QLineEdit();
    readerEmailEdit->setPlaceholderText("请输入邮箱");
    
    readerFormLayout->addRow("读者ID:", readerIdEdit);
    readerFormLayout->addRow("姓名:", readerNameEdit);
    readerFormLayout->addRow("电话:", readerPhoneEdit);
    readerFormLayout->addRow("邮箱:", readerEmailEdit);
    readerFormGroup->setLayout(readerFormLayout);
    
    QHBoxLayout *readerBtnLayout = new QHBoxLayout();
    QPushButton *addReaderBtn = new QPushButton("添加");
    addReaderBtn->setIcon(QIcon::fromTheme("list-add"));
    QPushButton *editReaderBtn = new QPushButton("修改");
    editReaderBtn->setIcon(QIcon::fromTheme("document-edit"));
    QPushButton *deleteReaderBtn = new QPushButton("删除");
    deleteReaderBtn->setIcon(QIcon::fromTheme("list-remove"));
    QPushButton *clearReaderBtn = new QPushButton("清空");
    
    addReaderBtn->setObjectName("addReaderBtn");
    editReaderBtn->setObjectName("editReaderBtn");
    deleteReaderBtn->setObjectName("deleteReaderBtn");
    clearReaderBtn->setObjectName("clearReaderBtn");

    readerLayout->addWidget(readerTable, 3);
    readerLayout->addLayout(readerRightLayout, 2);
    
    // ============ 借阅管理标签页 ============
    QWidget *borrowTab = new QWidget();
    QVBoxLayout *borrowLayout = new QVBoxLayout(borrowTab);
    
    // 借阅操作区域
    QGroupBox *borrowOperGroup = new QGroupBox("借阅操作");
    QHBoxLayout *borrowOperLayout = new QHBoxLayout();
    
    QLineEdit *borrowReaderIdEdit = new QLineEdit();
    borrowReaderIdEdit->setPlaceholderText("读者ID");
    QLineEdit *borrowIsbnEdit = new QLineEdit();
    borrowIsbnEdit->setPlaceholderText("图书ISBN");
    QSpinBox *borrowDaysSpin = new QSpinBox();
    borrowDaysSpin->setRange(1, 30);
    borrowDaysSpin->setValue(7);
    borrowDaysSpin->setPrefix("借阅天数: ");
    
    QPushButton *borrowBtn = new QPushButton("借书");
    borrowBtn->setIcon(QIcon::fromTheme("document-new"));
    borrowBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 8px 16px; }");
    QPushButton *returnBtn = new QPushButton("还书");
    returnBtn->setIcon(QIcon::fromTheme("document-save"));
    returnBtn->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 8px 16px; }");
    QPushButton *refreshBorrowBtn = new QPushButton("刷新列表");
    refreshBorrowBtn->setIcon(QIcon::fromTheme("view-refresh"));

    borrowBtn->setObjectName("borrowBtn");
    returnBtn->setObjectName("returnBtn");
    refreshBorrowBtn->setObjectName("refreshBorrowBtn");
    
    borrowOperLayout->addWidget(new QLabel("读者ID:"));
    borrowOperLayout->addWidget(borrowReaderIdEdit);
    borrowOperLayout->addWidget(new QLabel("ISBN:"));
    borrowOperLayout->addWidget(borrowIsbnEdit);
    borrowOperLayout->addWidget(borrowDaysSpin);
    borrowOperLayout->addWidget(borrowBtn);
    borrowOperLayout->addWidget(returnBtn);
    borrowOperLayout->addWidget(refreshBorrowBtn);
    borrowOperGroup->setLayout(borrowOperLayout);
    
    // 借阅记录表格
    QTabWidget *borrowTableTabs = new QTabWidget();
    
    QTableWidget *activeBorrowTable = new QTableWidget();
    activeBorrowTable->setColumnCount(7);
    activeBorrowTable->setHorizontalHeaderLabels({
        "读者ID", "ISBN", "借阅日期", "应还日期", "状态", "罚款(元)", "还书日期"
    });
    activeBorrowTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    activeBorrowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    activeBorrowTable->setAlternatingRowColors(true);
    
    QTableWidget *allBorrowTable = new QTableWidget();
    allBorrowTable->setColumnCount(7);
    allBorrowTable->setHorizontalHeaderLabels({
        "读者ID", "ISBN", "借阅日期", "应还日期", "状态", "罚款(元)", "还书日期"
    });
    allBorrowTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    allBorrowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    allBorrowTable->setAlternatingRowColors(true);
    
    borrowTableTabs->addTab(activeBorrowTable, "当前借阅");
    borrowTableTabs->addTab(allBorrowTable, "所有记录");
    
    borrowLayout->addWidget(borrowOperGroup);
    borrowLayout->addWidget(borrowTableTabs);
    
    // 添加标签页
    tabWidget->addTab(bookTab, "📚 图书管理");
    tabWidget->addTab(readerTab, "👥 读者管理");
    tabWidget->addTab(borrowTab, "📖 借阅管理");
    
    // 设置对象名称以便后续查找
    searchType->setObjectName("searchType");
    searchInput->setObjectName("searchInput");
    bookTable->setObjectName("bookTable");
    bookTitleEdit->setObjectName("bookTitleEdit");
    bookAuthorEdit->setObjectName("bookAuthorEdit");
    bookIsbnEdit->setObjectName("bookIsbnEdit");
    bookYearSpin->setObjectName("bookYearSpin");
    bookStockSpin->setObjectName("bookStockSpin");
    
    readerTable->setObjectName("readerTable");
    readerIdEdit->setObjectName("readerIdEdit");
    readerNameEdit->setObjectName("readerNameEdit");
    readerPhoneEdit->setObjectName("readerPhoneEdit");
    readerEmailEdit->setObjectName("readerEmailEdit");
    
    borrowReaderIdEdit->setObjectName("borrowReaderIdEdit");
    borrowIsbnEdit->setObjectName("borrowIsbnEdit");
    borrowDaysSpin->setObjectName("borrowDaysSpin");
    activeBorrowTable->setObjectName("activeBorrowTable");
    allBorrowTable->setObjectName("allBorrowTable");
    
    // 状态栏
    statusBar()->showMessage("就绪");
}

void MainWindow::setupConnections()
{
    // 搜索功能
    connect(findChild<QPushButton*>("searchBtn"), &QPushButton::clicked,
            this, &MainWindow::onSearchBook);
    connect(findChild<QPushButton*>("showAllBtn"), &QPushButton::clicked,
            [this]() { refreshBookTable(m_bookManager.getAllBooks()); });
    
    // 图书管理按钮
    connect(findChild<QPushButton*>("addBookBtn"), &QPushButton::clicked,
            this, &MainWindow::onAddBook);
    connect(findChild<QPushButton*>("editBookBtn"), &QPushButton::clicked,
            this, &MainWindow::onEditBook);
    connect(findChild<QPushButton*>("deleteBookBtn"), &QPushButton::clicked,
            this, &MainWindow::onDeleteBook);
    connect(findChild<QPushButton*>("clearBookBtn"), &QPushButton::clicked,
            this, &MainWindow::clearBookForm);
    
    // 图书表格选择
    connect(findChild<QTableWidget*>("bookTable"), &QTableWidget::itemSelectionChanged,
            this, &MainWindow::onBookTableSelectionChanged);
    
    // 读者管理按钮
    connect(findChild<QPushButton*>("addReaderBtn"), &QPushButton::clicked,
            this, &MainWindow::onAddReader);
    connect(findChild<QPushButton*>("editReaderBtn"), &QPushButton::clicked,
            this, &MainWindow::onEditReader);
    connect(findChild<QPushButton*>("deleteReaderBtn"), &QPushButton::clicked,
            this, &MainWindow::onDeleteReader);
    connect(findChild<QPushButton*>("clearReaderBtn"), &QPushButton::clicked,
            this, &MainWindow::clearReaderForm);
    
    // 读者表格选择
    connect(findChild<QTableWidget*>("readerTable"), &QTableWidget::itemSelectionChanged,
            this, &MainWindow::onReaderTableSelectionChanged);
    
    // 借阅管理按钮
    connect(findChild<QPushButton*>("borrowBtn"), &QPushButton::clicked,
            this, &MainWindow::onBorrowBook);
    connect(findChild<QPushButton*>("returnBtn"), &QPushButton::clicked,
            this, &MainWindow::onReturnBook);
    connect(findChild<QPushButton*>("refreshBorrowBtn"), &QPushButton::clicked,
            this, &MainWindow::onRefreshBorrowList);
}

void MainWindow::setupTableStyles()
{
    QString tableStyle = R"(
        QTableWidget {
            border: 1px solid #ddd;
            border-radius: 4px;
            background-color: white;
            gridline-color: #f0f0f0;
        }
        QTableWidget::item {
            padding: 5px;
        }
        QTableWidget::item:selected {
            background-color: #0078d7;
            color: white;
        }
        QHeaderView::section {
            background-color: #f8f9fa;
            padding: 8px;
            border: none;
            border-bottom: 2px solid #dee2e6;
            font-weight: bold;
        }
    )";
    
    QTableWidget* bookTable = findChild<QTableWidget*>("bookTable");
    QTableWidget* readerTable = findChild<QTableWidget*>("readerTable");
    QTableWidget* activeBorrowTable = findChild<QTableWidget*>("activeBorrowTable");
    QTableWidget* allBorrowTable = findChild<QTableWidget*>("allBorrowTable");
    
    if (bookTable) bookTable->setStyleSheet(tableStyle);
    if (readerTable) readerTable->setStyleSheet(tableStyle);
    if (activeBorrowTable) activeBorrowTable->setStyleSheet(tableStyle);
    if (allBorrowTable) allBorrowTable->setStyleSheet(tableStyle);
}

void MainWindow::loadAllData()
{
    m_bookManager.loadFromFile(BOOK_FILE);
    m_readerManager.loadFromFile(READER_FILE);
    m_borrowManager.loadFromFile(BORROW_FILE);
    
    statusBar()->showMessage("数据加载完成", 3000);
}

void MainWindow::saveAllData()
{
    m_bookManager.saveToFile(BOOK_FILE);
    m_readerManager.saveToFile(READER_FILE);
    m_borrowManager.saveToFile(BORROW_FILE);
    
    statusBar()->showMessage("数据保存完成", 3000);
}

void MainWindow::refreshBookTable(const QList<Book> &books)
{
    QTableWidget* bookTable = findChild<QTableWidget*>("bookTable");
    if (!bookTable) return;
    
    bookTable->setRowCount(books.size());
    
    for (int i = 0; i < books.size(); ++i) {
        const Book &book = books[i];
        bookTable->setItem(i, 0, new QTableWidgetItem(book.getTitle()));
        bookTable->setItem(i, 1, new QTableWidgetItem(book.getAuthor()));
        bookTable->setItem(i, 2, new QTableWidgetItem(book.getIsbn()));
        bookTable->setItem(i, 3, new QTableWidgetItem(QString::number(book.getYear())));
        bookTable->setItem(i, 4, new QTableWidgetItem(QString::number(book.getStock())));
        
        // 库存为0时显示红色
        if (book.getStock() == 0) {
            bookTable->item(i, 4)->setForeground(Qt::red);
        }
    }
    
    updateStatusBar();
}

void MainWindow::refreshReaderTable()
{
    QTableWidget* readerTable = findChild<QTableWidget*>("readerTable");
    if (!readerTable) return;
    
    QList<Reader> readers = m_readerManager.getAllReaders();
    readerTable->setRowCount(readers.size());
    
    for (int i = 0; i < readers.size(); ++i) {
        const Reader &reader = readers[i];
        readerTable->setItem(i, 0, new QTableWidgetItem(reader.getId()));
        readerTable->setItem(i, 1, new QTableWidgetItem(reader.getName()));
        readerTable->setItem(i, 2, new QTableWidgetItem(reader.getPhone()));
        readerTable->setItem(i, 3, new QTableWidgetItem(reader.getEmail()));
    }
}

void MainWindow::refreshBorrowTable()
{
    QTableWidget* activeBorrowTable = findChild<QTableWidget*>("activeBorrowTable");
    QTableWidget* allBorrowTable = findChild<QTableWidget*>("allBorrowTable");
    
    if (!activeBorrowTable || !allBorrowTable) return;
    
    // 刷新活跃借阅
    QList<BorrowRecord> activeBorrows = m_borrowManager.getActiveBorrows();
    activeBorrowTable->setRowCount(activeBorrows.size());
    
    for (int i = 0; i < activeBorrows.size(); ++i) {
        const BorrowRecord &record = activeBorrows[i];
        activeBorrowTable->setItem(i, 0, new QTableWidgetItem(record.getReaderId()));
        activeBorrowTable->setItem(i, 1, new QTableWidgetItem(record.getIsbn()));
        activeBorrowTable->setItem(i, 2, new QTableWidgetItem(record.getBorrowDate().toString("yyyy-MM-dd")));
        activeBorrowTable->setItem(i, 3, new QTableWidgetItem(record.getDueDate().toString("yyyy-MM-dd")));
        activeBorrowTable->setItem(i, 4, new QTableWidgetItem("借阅中"));
        
        double fine = record.getFine();
        QTableWidgetItem *fineItem = new QTableWidgetItem(QString::number(fine, 'f', 2));
        if (fine > 0) {
            fineItem->setForeground(Qt::red);
        }
        activeBorrowTable->setItem(i, 5, fineItem);
        activeBorrowTable->setItem(i, 6, new QTableWidgetItem("-"));
    }
    
    // 刷新所有记录
    QList<BorrowRecord> allBorrows = m_borrowManager.getAllBorrows();
    allBorrowTable->setRowCount(allBorrows.size());
    
    for (int i = 0; i < allBorrows.size(); ++i) {
        const BorrowRecord &record = allBorrows[i];
        allBorrowTable->setItem(i, 0, new QTableWidgetItem(record.getReaderId()));
        allBorrowTable->setItem(i, 1, new QTableWidgetItem(record.getIsbn()));
        allBorrowTable->setItem(i, 2, new QTableWidgetItem(record.getBorrowDate().toString("yyyy-MM-dd")));
        allBorrowTable->setItem(i, 3, new QTableWidgetItem(record.getDueDate().toString("yyyy-MM-dd")));
        
        if (record.isReturned()) {
            allBorrowTable->setItem(i, 4, new QTableWidgetItem("已归还"));
            allBorrowTable->item(i, 4)->setForeground(QColor("#4CAF50"));
            allBorrowTable->setItem(i, 6, new QTableWidgetItem(record.getReturnDate().toString("yyyy-MM-dd")));
        } else {
            allBorrowTable->setItem(i, 4, new QTableWidgetItem("借阅中"));
            allBorrowTable->setItem(i, 6, new QTableWidgetItem("-"));
        }
        
        double fine = record.getFine();
        QTableWidgetItem *fineItem = new QTableWidgetItem(QString::number(fine, 'f', 2));
        if (fine > 0) {
            fineItem->setForeground(Qt::red);
        }
        allBorrowTable->setItem(i, 5, fineItem);
    }
}

void MainWindow::clearBookForm()
{
    findChild<QLineEdit*>("bookTitleEdit")->clear();
    findChild<QLineEdit*>("bookAuthorEdit")->clear();
    findChild<QLineEdit*>("bookIsbnEdit")->clear();
    findChild<QSpinBox*>("bookYearSpin")->setValue(2024);
    findChild<QSpinBox*>("bookStockSpin")->setValue(1);
    m_selectedBookRow = -1;
}

void MainWindow::clearReaderForm()
{
    findChild<QLineEdit*>("readerIdEdit")->clear();
    findChild<QLineEdit*>("readerNameEdit")->clear();
    findChild<QLineEdit*>("readerPhoneEdit")->clear();
    findChild<QLineEdit*>("readerEmailEdit")->clear();
    m_selectedReaderRow = -1;
}

void MainWindow::updateStatusBar()
{
    int totalBooks = m_bookManager.getAllBooks().size();
    int totalReaders = m_readerManager.getAllReaders().size();
    int activeBorrows = m_borrowManager.getActiveBorrows().size();
    
    statusBar()->showMessage(QString("图书总数: %1 | 读者总数: %2 | 当前借阅: %3")
                            .arg(totalBooks).arg(totalReaders).arg(activeBorrows));
}

// ============ 图书管理槽函数 ============

void MainWindow::onAddBook()
{
    QLineEdit* titleEdit = findChild<QLineEdit*>("bookTitleEdit");
    QLineEdit* authorEdit = findChild<QLineEdit*>("bookAuthorEdit");
    QLineEdit* isbnEdit = findChild<QLineEdit*>("bookIsbnEdit");
    QSpinBox* yearSpin = findChild<QSpinBox*>("bookYearSpin");
    QSpinBox* stockSpin = findChild<QSpinBox*>("bookStockSpin");
    
    QString title = titleEdit->text().trimmed();
    QString author = authorEdit->text().trimmed();
    QString isbn = isbnEdit->text().trimmed();
    int year = yearSpin->value();
    int stock = stockSpin->value();
    
    if (title.isEmpty() || author.isEmpty() || isbn.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请填写所有必填字段！");
        return;
    }
    
    Book book(title, author, isbn, year, stock);
    
    if (m_bookManager.addBook(book)) {
        refreshBookTable(m_bookManager.getAllBooks());
        clearBookForm();
        saveAllData();
        QMessageBox::information(this, "成功", "图书添加成功！");
    } else {
        QMessageBox::warning(this, "错误", "ISBN已存在，添加失败！");
    }
}

void MainWindow::onEditBook()
{
    QTableWidget* bookTable = findChild<QTableWidget*>("bookTable");
    if (!bookTable || m_selectedBookRow < 0) {
        QMessageBox::warning(this, "提示", "请先选择要修改的图书！");
        return;
    }
    
    QLineEdit* titleEdit = findChild<QLineEdit*>("bookTitleEdit");
    QLineEdit* authorEdit = findChild<QLineEdit*>("bookAuthorEdit");
    QLineEdit* isbnEdit = findChild<QLineEdit*>("bookIsbnEdit");
    QSpinBox* yearSpin = findChild<QSpinBox*>("bookYearSpin");
    QSpinBox* stockSpin = findChild<QSpinBox*>("bookStockSpin");
    
    QString originalIsbn = bookTable->item(m_selectedBookRow, 2)->text();
    QString title = titleEdit->text().trimmed();
    QString author = authorEdit->text().trimmed();
    QString isbn = isbnEdit->text().trimmed();
    int year = yearSpin->value();
    int stock = stockSpin->value();
    
    if (title.isEmpty() || author.isEmpty() || isbn.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请填写所有必填字段！");
        return;
    }
    
    Book book(title, author, isbn, year, stock);
    
    // 如果ISBN改变了，需要删除旧的并添加新的
    if (originalIsbn != isbn) {
        m_bookManager.removeBook(originalIsbn);
        if (m_bookManager.addBook(book)) {
            refreshBookTable(m_bookManager.getAllBooks());
            clearBookForm();
            saveAllData();
            QMessageBox::information(this, "成功", "图书修改成功！");
        } else {
            // 恢复原记录
            Book originalBook(bookTable->item(m_selectedBookRow, 0)->text(),
                            bookTable->item(m_selectedBookRow, 1)->text(),
                            originalIsbn,
                            bookTable->item(m_selectedBookRow, 3)->text().toInt(),
                            bookTable->item(m_selectedBookRow, 4)->text().toInt());
            m_bookManager.addBook(originalBook);
            QMessageBox::warning(this, "错误", "修改失败，新ISBN可能已存在！");
        }
    } else {
        if (m_bookManager.updateBook(book)) {
            refreshBookTable(m_bookManager.getAllBooks());
            clearBookForm();
            saveAllData();
            QMessageBox::information(this, "成功", "图书修改成功！");
        } else {
            QMessageBox::warning(this, "错误", "修改失败！");
        }
    }
}

void MainWindow::onDeleteBook()
{
    QTableWidget* bookTable = findChild<QTableWidget*>("bookTable");
    if (!bookTable || m_selectedBookRow < 0) {
        QMessageBox::warning(this, "提示", "请先选择要删除的图书！");
        return;
    }
    
    QString isbn = bookTable->item(m_selectedBookRow, 2)->text();
    QString title = bookTable->item(m_selectedBookRow, 0)->text();
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "确认删除",
        QString("确定要删除图书《%1》吗？").arg(title),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (m_bookManager.removeBook(isbn)) {
            refreshBookTable(m_bookManager.getAllBooks());
            clearBookForm();
            saveAllData();
            QMessageBox::information(this, "成功", "图书删除成功！");
        }
    }
}

void MainWindow::onSearchBook()
{
    QComboBox* searchType = findChild<QComboBox*>("searchType");
    QLineEdit* searchInput = findChild<QLineEdit*>("searchInput");
    
    if (!searchType || !searchInput) return;
    
    QString keyword = searchInput->text().trimmed();
    if (keyword.isEmpty()) {
        refreshBookTable(m_bookManager.getAllBooks());
        return;
    }
    
    QList<Book> results;
    switch (searchType->currentIndex()) {
        case 0: // 书名
            results = m_bookManager.searchByTitle(keyword);
            break;
        case 1: // 作者
            results = m_bookManager.searchByAuthor(keyword);
            break;
        case 2: // ISBN
            results = m_bookManager.searchByIsbn(keyword);
            break;
    }
    
    refreshBookTable(results);
    statusBar()->showMessage(QString("找到 %1 条结果").arg(results.size()), 3000);
}

void MainWindow::onBookTableSelectionChanged()
{
    QTableWidget* bookTable = findChild<QTableWidget*>("bookTable");
    if (!bookTable) return;
    
    QList<QTableWidgetItem*> selectedItems = bookTable->selectedItems();
    if (selectedItems.isEmpty()) return;
    
    m_selectedBookRow = selectedItems.first()->row();
    
    // 填充表单
    findChild<QLineEdit*>("bookTitleEdit")->setText(bookTable->item(m_selectedBookRow, 0)->text());
    findChild<QLineEdit*>("bookAuthorEdit")->setText(bookTable->item(m_selectedBookRow, 1)->text());
    findChild<QLineEdit*>("bookIsbnEdit")->setText(bookTable->item(m_selectedBookRow, 2)->text());
    findChild<QSpinBox*>("bookYearSpin")->setValue(bookTable->item(m_selectedBookRow, 3)->text().toInt());
    findChild<QSpinBox*>("bookStockSpin")->setValue(bookTable->item(m_selectedBookRow, 4)->text().toInt());
}

// ============ 读者管理槽函数 ============

void MainWindow::onAddReader()
{
    QLineEdit* idEdit = findChild<QLineEdit*>("readerIdEdit");
    QLineEdit* nameEdit = findChild<QLineEdit*>("readerNameEdit");
    QLineEdit* phoneEdit = findChild<QLineEdit*>("readerPhoneEdit");
    QLineEdit* emailEdit = findChild<QLineEdit*>("readerEmailEdit");
    
    QString id = idEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();
    
    if (id.isEmpty() || name.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请填写读者ID和姓名！");
        return;
    }
    
    Reader reader(id, name, phone, email);
    
    if (m_readerManager.addReader(reader)) {
        refreshReaderTable();
        clearReaderForm();
        saveAllData();
        QMessageBox::information(this, "成功", "读者添加成功！");
    } else {
        QMessageBox::warning(this, "错误", "读者ID已存在！");
    }
}

void MainWindow::onEditReader()
{
    QTableWidget* readerTable = findChild<QTableWidget*>("readerTable");
    if (!readerTable || m_selectedReaderRow < 0) {
        QMessageBox::warning(this, "提示", "请先选择要修改的读者！");
        return;
    }
    
    QLineEdit* idEdit = findChild<QLineEdit*>("readerIdEdit");
    QLineEdit* nameEdit = findChild<QLineEdit*>("readerNameEdit");
    QLineEdit* phoneEdit = findChild<QLineEdit*>("readerPhoneEdit");
    QLineEdit* emailEdit = findChild<QLineEdit*>("readerEmailEdit");
    
    QString originalId = readerTable->item(m_selectedReaderRow, 0)->text();
    QString id = idEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();
    
    if (id.isEmpty() || name.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请填写读者ID和姓名！");
        return;
    }
    
    Reader reader(id, name, phone, email);
    
    if (originalId != id) {
        m_readerManager.removeReader(originalId);
        if (m_readerManager.addReader(reader)) {
            refreshReaderTable();
            clearReaderForm();
            saveAllData();
            QMessageBox::information(this, "成功", "读者信息修改成功！");
        } else {
            QMessageBox::warning(this, "错误", "修改失败！");
        }
    } else {
        if (m_readerManager.updateReader(reader)) {
            refreshReaderTable();
            clearReaderForm();
            saveAllData();
            QMessageBox::information(this, "成功", "读者信息修改成功！");
        } else {
            QMessageBox::warning(this, "错误", "修改失败！");
        }
    }
}

void MainWindow::onDeleteReader()
{
    QTableWidget* readerTable = findChild<QTableWidget*>("readerTable");
    if (!readerTable || m_selectedReaderRow < 0) {
        QMessageBox::warning(this, "提示", "请先选择要删除的读者！");
        return;
    }
    
    QString id = readerTable->item(m_selectedReaderRow, 0)->text();
    QString name = readerTable->item(m_selectedReaderRow, 1)->text();
    
    // 检查是否有未归还的图书
    if (m_borrowManager.getActiveBorrowCount(id) > 0) {
        QMessageBox::warning(this, "无法删除",
            QString("读者 %1 还有未归还的图书，请先归还所有图书！").arg(name));
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "确认删除",
        QString("确定要删除读者 %1 吗？").arg(name),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (m_readerManager.removeReader(id)) {
            refreshReaderTable();
            clearReaderForm();
            saveAllData();
            QMessageBox::information(this, "成功", "读者删除成功！");
        }
    }
}

void MainWindow::onReaderTableSelectionChanged()
{
    QTableWidget* readerTable = findChild<QTableWidget*>("readerTable");
    if (!readerTable) return;
    
    QList<QTableWidgetItem*> selectedItems = readerTable->selectedItems();
    if (selectedItems.isEmpty()) return;
    
    m_selectedReaderRow = selectedItems.first()->row();
    
    findChild<QLineEdit*>("readerIdEdit")->setText(readerTable->item(m_selectedReaderRow, 0)->text());
    findChild<QLineEdit*>("readerNameEdit")->setText(readerTable->item(m_selectedReaderRow, 1)->text());
    findChild<QLineEdit*>("readerPhoneEdit")->setText(readerTable->item(m_selectedReaderRow, 2)->text());
    findChild<QLineEdit*>("readerEmailEdit")->setText(readerTable->item(m_selectedReaderRow, 3)->text());
}

// ============ 借阅管理槽函数 ============

void MainWindow::onBorrowBook()
{
    QLineEdit* readerIdEdit = findChild<QLineEdit*>("borrowReaderIdEdit");
    QLineEdit* isbnEdit = findChild<QLineEdit*>("borrowIsbnEdit");
    QSpinBox* daysSpin = findChild<QSpinBox*>("borrowDaysSpin");
    
    QString readerId = readerIdEdit->text().trimmed();
    QString isbn = isbnEdit->text().trimmed();
    int borrowDays = daysSpin->value();
    
    if (readerId.isEmpty() || isbn.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入读者ID和图书ISBN！");
        return;
    }
    
    // 检查读者是否存在
    Reader* reader = m_readerManager.findReader(readerId);
    if (!reader) {
        QMessageBox::warning(this, "错误", "读者不存在，请先注册读者信息！");
        return;
    }
    
    // 检查图书是否存在
    Book* book = m_bookManager.findBook(isbn);
    if (!book) {
        QMessageBox::warning(this, "错误", "图书不存在！");
        return;
    }
    
    // 检查库存
    if (book->getStock() <= 0) {
        QMessageBox::warning(this, "错误", "图书库存不足！");
        return;
    }
    
    // 检查借阅数量（最多3本）
    int currentBorrows = m_borrowManager.getActiveBorrowCount(readerId);
    if (currentBorrows >= 3) {
        QMessageBox::warning(this, "错误",
            QString("读者 %1 已达到最大借阅数量（3本）！").arg(reader->getName()));
        return;
    }
    
    // 检查是否已经借过这本书
    if (m_borrowManager.findActiveBorrow(readerId, isbn)) {
        QMessageBox::warning(this, "错误", "该读者已经借阅了这本书，不能重复借阅！");
        return;
    }
    
    // 执行借阅
    if (m_borrowManager.borrowBook(readerId, isbn, borrowDays)) {
        book->decreaseStock();
        refreshBookTable(m_bookManager.getAllBooks());
        refreshBorrowTable();
        saveAllData();
        
        QMessageBox::information(this, "成功",
            QString("借阅成功！\n读者: %1\n图书: %2\n应还日期: %3")
            .arg(reader->getName())
            .arg(book->getTitle())
            .arg(QDate::currentDate().addDays(borrowDays).toString("yyyy-MM-dd")));
        
        readerIdEdit->clear();
        isbnEdit->clear();
    }
}

void MainWindow::onReturnBook()
{
    QLineEdit* readerIdEdit = findChild<QLineEdit*>("borrowReaderIdEdit");
    QLineEdit* isbnEdit = findChild<QLineEdit*>("borrowIsbnEdit");
    
    QString readerId = readerIdEdit->text().trimmed();
    QString isbn = isbnEdit->text().trimmed();
    
    if (readerId.isEmpty() || isbn.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入读者ID和图书ISBN！");
        return;
    }
    
    // 查找借阅记录
    BorrowRecord* record = m_borrowManager.findActiveBorrow(readerId, isbn);
    if (!record) {
        QMessageBox::warning(this, "错误", "未找到该借阅记录！");
        return;
    }
    
    // 计算罚款
    double fine = record->calculateFine();
    QString fineMsg;
    if (fine > 0) {
        int overdueDays = record->getDueDate().daysTo(QDate::currentDate());
        fineMsg = QString("\n逾期天数: %1 天\n罚款金额: %2 元").arg(overdueDays).arg(fine, 0, 'f', 2);
    }
    
    // 执行归还
    if (m_borrowManager.returnBook(readerId, isbn)) {
        Book* book = m_bookManager.findBook(isbn);
        if (book) {
            book->increaseStock();
        }
        
        refreshBookTable(m_bookManager.getAllBooks());
        refreshBorrowTable();
        saveAllData();
        
        QMessageBox::information(this, "成功",
            QString("归还成功！%1").arg(fineMsg));
        
        readerIdEdit->clear();
        isbnEdit->clear();
    }
}

void MainWindow::onRefreshBorrowList()
{
    refreshBorrowTable();
    statusBar()->showMessage("借阅列表已刷新", 3000);
}

void MainWindow::onTabChanged(int index)
{
    Q_UNUSED(index);
    // 这里可以处理标签页切换时的逻辑
}