#include "MainWindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QApplication>
#include <QScreen>
#include <QFormLayout>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_manager = StudentManager::getInstance();

    if(!m_manager->isDBConnectOk())
    {
        QMessageBox::critical(this,"错误","数据库连接失败！请检查服务、账号密码。");
    }

    initUI();
    setUIStyle();
    refreshTable();

    this->setWindowTitle("学生成绩管理系统 V3.0");
    this->setFixedSize(1200, 800);
    QScreen *screen = QApplication::primaryScreen();
    QRect rect = screen->availableGeometry();
    this->move((rect.width()-width())/2,(rect.height()-height())/2);
}

MainWindow::~MainWindow()
{
    StudentManager::releaseInstance();
}

bool MainWindow::checkInputEmpty()
{
    if(edtId->text().trimmed().isEmpty()){QMessageBox::warning(this,"提示","学号不能为空！");return false;}
    if(edtName->text().trimmed().isEmpty()){QMessageBox::warning(this,"提示","姓名不能为空！");return false;}
    if(edtClass->text().trimmed().isEmpty()){QMessageBox::warning(this,"提示","班级不能为空！");return false;}
    if(edtCourse->text().trimmed().isEmpty()){QMessageBox::warning(this,"提示","课程不能为空！");return false;}
    return true;
}

void MainWindow::initUI()
{
    QWidget* cw = new QWidget(this);
    setCentralWidget(cw);
    QVBoxLayout* mainLayout = new QVBoxLayout(cw);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30,30,30,30);

    // 1. 学生信息录入模块
    QGroupBox* g1 = new QGroupBox("📝 学生信息录入");
    QFormLayout* f1 = new QFormLayout(g1);
    f1->setSpacing(15);
    f1->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    edtId = new QLineEdit;
    edtName = new QLineEdit;
    edtClass = new QLineEdit;
    edtCourse = new QLineEdit;
    spinScore = new QDoubleSpinBox;
    spinScore->setRange(0,100);
    spinScore->setDecimals(1);
    spinScore->setSingleStep(0.5);

    f1->addRow("学号：",edtId);
    f1->addRow("姓名：",edtName);
    f1->addRow("班级：",edtClass);
    f1->addRow("课程：",edtCourse);
    f1->addRow("成绩：",spinScore);

    QHBoxLayout* b1 = new QHBoxLayout;
    b1->setSpacing(15);
    btnAdd = new QPushButton("添加学生");
    btnUpdate = new QPushButton("修改信息");
    btnDelete = new QPushButton("删除学生");
    btnClear = new QPushButton("清空输入");
    b1->addWidget(btnAdd); b1->addWidget(btnUpdate); b1->addWidget(btnDelete); b1->addWidget(btnClear);
    f1->addRow("",b1);
    mainLayout->addWidget(g1);

    // 2. 学生信息查询模块
    QGroupBox* g2 = new QGroupBox("🔍 学生信息查询");
    QHBoxLayout* h2 = new QHBoxLayout(g2);
    h2->setSpacing(15);
    cboSearchType = new QComboBox;
    cboSearchType->addItems({"学号精确查询","姓名精确查询","班级模糊查询","课程模糊查询"});
    edtSearch = new QLineEdit;
    edtSearch->setPlaceholderText("请输入查询关键词");
    btnSearch = new QPushButton("立即查询");
    btnShowAll = new QPushButton("重置全部");
    h2->addWidget(cboSearchType); h2->addWidget(edtSearch); h2->addWidget(btnSearch); h2->addWidget(btnShowAll);
    mainLayout->addWidget(g2);

    // 3. 成绩统计与排序模块
    QGroupBox* g3 = new QGroupBox("📊 成绩统计与排序");
    QHBoxLayout* h3 = new QHBoxLayout(g3);
    h3->setSpacing(15);
    edtStatCourse = new QLineEdit;
    edtStatCourse->setPlaceholderText("输入课程名称进行统计");
    btnStat = new QPushButton("统计成绩");
    labStatResult = new QLabel("统计结果：暂无数据");
    btnSortAsc = new QPushButton("成绩升序");
    btnSortDesc = new QPushButton("成绩降序");
    h3->addWidget(edtStatCourse); h3->addWidget(btnStat); h3->addWidget(labStatResult);
    h3->addStretch(); h3->addWidget(btnSortAsc); h3->addWidget(btnSortDesc);
    mainLayout->addWidget(g3);

    // 4. 表格模块（添加选择功能）
    tableWidget = new QTableWidget;
    tableWidget->setColumnCount(5);
    tableWidget->setHorizontalHeaderLabels({"学号","姓名","班级","课程","成绩"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选择
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); // 单选
    tableWidget->setAlternatingRowColors(true); // 交替行颜色
    tableWidget->verticalHeader()->setVisible(false); // 隐藏行号
    mainLayout->addWidget(tableWidget);

    // 连接信号槽
    connect(btnAdd,&QPushButton::clicked,this,&MainWindow::addBtnClicked);
    connect(btnUpdate,&QPushButton::clicked,this,&MainWindow::updateBtnClicked);
    connect(btnDelete,&QPushButton::clicked,this,&MainWindow::deleteBtnClicked);
    connect(btnClear,&QPushButton::clicked,this,&MainWindow::clearBtnClicked);
    connect(btnSearch,&QPushButton::clicked,this,&MainWindow::searchBtnClicked);
    connect(btnStat,&QPushButton::clicked,this,&MainWindow::statBtnClicked);
    connect(btnSortAsc,&QPushButton::clicked,this,&MainWindow::sortAscBtnClicked);
    connect(btnSortDesc,&QPushButton::clicked,this,&MainWindow::sortDescBtnClicked);
    connect(btnShowAll,&QPushButton::clicked,this,&MainWindow::showAllData);
    connect(tableWidget,&QTableWidget::cellClicked,this,&MainWindow::tableCellClicked);
}

void MainWindow::setUIStyle()
{
    this->setStyleSheet(R"(
        QMainWindow{background-color:#F0F2F5;}
        QGroupBox{font-size:16px;font-weight:600;color:#1F2937;border:none;border-radius:12px;margin-top:10px;padding:20px;background-color:#FFFFFF;}
        QGroupBox::title{subcontrol-origin:margin;left:20px;top:8px;padding:0 8px;background-color:#F0F2F5;}
        QLineEdit,QDoubleSpinBox,QComboBox{border:1px solid #D1D5DB;border-radius:8px;padding:10px 15px;font-size:14px;min-height:35px;background-color:#FFFFFF;}
        QLineEdit:focus,QDoubleSpinBox:focus,QComboBox:focus{border:2px solid #3B82F6;}
        QPushButton{font-size:14px;font-weight:500;padding:10px 20px;border-radius:8px;color:#fff;border:none;min-height:35px;}
        QPushButton:hover{opacity:0.9;}
        QPushButton:pressed{opacity:0.8;}
        QPushButton#btnAdd{background-color:#10B981;}
        QPushButton#btnUpdate{background-color:#3B82F6;}
        QPushButton#btnDelete{background-color:#EF4444;}
        QPushButton#btnClear{background-color:#6B7280;}
        QPushButton#btnSearch{background-color:#8B5CF6;}
        QPushButton#btnShowAll{background-color:#22C55E;}
        QPushButton#btnStat{background-color:#F59E0B;}
        QPushButton#btnSortAsc,QPushButton#btnSortDesc{background-color:#14B8A6;}
        QTableWidget{border:none;border-radius:12px;gridline-color:#E5E7EB;background-color:#fff;alternate-background-color:#F9FAFB;}
        QTableWidget::item{padding:12px;font-size:14px;}
        QTableWidget::item:selected{background-color:#DBEAFE;color:#1F2937;font-weight:500;}
        QHeaderView::section{background-color:#3B82F6;color:#fff;border:none;padding:12px;font-size:14px;font-weight:600;}
        QLabel{font-size:14px;color:#374151;font-weight:500;}
    )");
    btnAdd->setObjectName("btnAdd");
    btnUpdate->setObjectName("btnUpdate");
    btnDelete->setObjectName("btnDelete");
    btnClear->setObjectName("btnClear");
    btnSearch->setObjectName("btnSearch");
    btnShowAll->setObjectName("btnShowAll");
    btnStat->setObjectName("btnStat");
    btnSortAsc->setObjectName("btnSortAsc");
    btnSortDesc->setObjectName("btnSortDesc");
}

void MainWindow::refreshTable(const QList<Student>& list)
{
    QList<Student> data = list.isEmpty() ? m_manager->getAllStudents() : list;
    tableWidget->setRowCount(0);
    if(data.isEmpty()) return;
    for(int i=0;i<data.size();i++)
    {
        tableWidget->insertRow(i);
        tableWidget->setItem(i,0,new QTableWidgetItem(data[i].id));
        tableWidget->setItem(i,1,new QTableWidgetItem(data[i].name));
        tableWidget->setItem(i,2,new QTableWidgetItem(data[i].className));
        tableWidget->setItem(i,3,new QTableWidgetItem(data[i].course));
        tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(data[i].score,'f',1)));
    }
}

void MainWindow::tableCellClicked(int row, int col)
{
    Q_UNUSED(col);
    edtId->setText(tableWidget->item(row,0)->text());
    edtName->setText(tableWidget->item(row,1)->text());
    edtClass->setText(tableWidget->item(row,2)->text());
    edtCourse->setText(tableWidget->item(row,3)->text());
    spinScore->setValue(tableWidget->item(row,4)->text().toDouble());
}

void MainWindow::showAllData()
{
    edtSearch->clear();
    labStatResult->setText("统计结果：暂无数据");
    refreshTable();
}

void MainWindow::addBtnClicked()
{
    if(!checkInputEmpty()) return;
    Student s(edtId->text(),edtName->text(),edtClass->text(),edtCourse->text(),spinScore->value());
    if(m_manager->addStudent(s))
    {
        QMessageBox::information(this,"成功","学生信息添加完成！");
        refreshTable();
        clearBtnClicked();
    }
    else
    {
        QMessageBox::warning(this,"失败","学号重复或数据库异常！");
    }
}

void MainWindow::updateBtnClicked()
{
    if(!checkInputEmpty()) return;
    Student s(edtId->text(),edtName->text(),edtClass->text(),edtCourse->text(),spinScore->value());
    if(m_manager->updateStudent(s))
    {
        QMessageBox::information(this,"成功","信息修改成功！");
        refreshTable();
    }
    else
    {
        QMessageBox::warning(this,"失败","未找到该学生信息！");
    }
}

void MainWindow::deleteBtnClicked()
{
    if(edtId->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this,"提示","请先选择要删除的学生！");
        return;
    }
    int ret = QMessageBox::question(this,"确认删除","确定要删除该学生数据吗？此操作不可恢复！",QMessageBox::Yes|QMessageBox::No);
    if(ret != QMessageBox::Yes) return;

    if(m_manager->deleteStudent(edtId->text()))
    {
        QMessageBox::information(this,"成功","删除完成！");
        refreshTable();
        clearBtnClicked();
    }
    else
    {
        QMessageBox::warning(this,"失败","删除失败！");
    }
}

void MainWindow::clearBtnClicked()
{
    edtId->clear();
    edtName->clear();
    edtClass->clear();
    edtCourse->clear();
    spinScore->setValue(0);
}

void MainWindow::searchBtnClicked()
{
    QString key = edtSearch->text().trimmed();
    QList<Student> res;
    switch (cboSearchType->currentIndex())
    {
        case 0:res=m_manager->searchById(key);break;
        case 1:res=m_manager->searchByName(key);break;
        case 2:res=m_manager->searchByClass(key);break;
        case 3:res=m_manager->searchByCourse(key);break;
    }
    if(res.isEmpty())
    {
        QMessageBox::information(this,"查询结果","未查询到匹配数据！");
    }
    refreshTable(res);
}

void MainWindow::statBtnClicked()
{
    QString course = edtStatCourse->text().trimmed();
    if(course.isEmpty())
    {
        QMessageBox::warning(this,"提示","请输入课程名称！");
        return;
    }
    auto map = m_manager->statistics(course);
    if(map["avg"] <= 0 && map["max"] <=0)
    {
        labStatResult->setText("统计结果：该课程暂无成绩数据");
        return;
    }
    QString res = QString("平均分:%.1f 最高分:%.1f 最低分:%.1f 及格率:%.1f%%")
            .arg(map["avg"]).arg(map["max"]).arg(map["min"]).arg(map["pass"]);
    labStatResult->setText(res);
}

void MainWindow::sortAscBtnClicked()
{
    refreshTable(m_manager->sortByScore(true));
}

void MainWindow::sortDescBtnClicked()
{
    refreshTable(m_manager->sortByScore(false));
}