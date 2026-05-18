#include "StudentManager.h"
#include <QSqlError>
#include <QDebug>
#include <algorithm>

StudentManager* StudentManager::instance = nullptr;

StudentManager::StudentManager()
{
    m_isConnected = connectDB();
}

StudentManager::~StudentManager()
{
    closeDB();
}

StudentManager* StudentManager::getInstance()
{
    if (!instance)
        instance = new StudentManager;
    return instance;
}

void StudentManager::releaseInstance()
{
    if(instance)
    {
        delete instance;
        instance = nullptr;
    }
}

bool StudentManager::connectDB()
{
    // 检查MySQL驱动
    if (!QSqlDatabase::isDriverAvailable("QMYSQL")) {
        qDebug() << "❌ 错误：Qt未加载MySQL驱动！";
        qDebug() << "可用驱动：" << QSqlDatabase::drivers();
        return false;
    }

    // 连接配置，和你的Navicat完全一致
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("student_score_db"); // ✅ 你的新数据库名
    db.setUserName("root");
    db.setPassword("123456"); // ❗ 改成你自己的密码

    bool ok = db.open();
    if (ok) {
        qDebug() << "✅ 数据库连接成功！";
        return true;
    } else {
        qDebug() << "❌ 连接失败：" << db.lastError().text();
        return false;
    }
}

void StudentManager::closeDB()
{
    if(db.isOpen())
        db.close();
}

bool StudentManager::isDBConnectOk()
{
    return m_isConnected;
}

QList<Student> StudentManager::getAllStudents()
{
    QList<Student> list;
    if(!m_isConnected) return list;

    QSqlQuery query;
    query.exec(R"(
        SELECT s.id, s.name, s.class, c.course_name, sc.score
        FROM student s
        LEFT JOIN score sc ON s.id = sc.student_id
        LEFT JOIN course c ON sc.course_id = c.course_id
    )");

    while(query.next())
    {
        Student stu;
        stu.id = query.value(0).toString();
        stu.name = query.value(1).toString();
        stu.className = query.value(2).toString();
        stu.course = query.value(3).toString();
        stu.score = query.value(4).toDouble();
        list.append(stu);
    }
    return list;
}

bool StudentManager::addStudent(const Student& stu)
{
    if(!m_isConnected) return false;
    QSqlQuery query;

    // 1. 插入学生信息到 student 表
    query.prepare("INSERT INTO student(id, name, class) VALUES(?,?,?)");
    query.addBindValue(stu.id);
    query.addBindValue(stu.name);
    query.addBindValue(stu.className);
    if(!query.exec()) return false;

    // 2. 从 course 表获取课程ID
    query.prepare("SELECT course_id FROM course WHERE course_name=?");
    query.addBindValue(stu.course);
    if(!query.exec() || !query.next()) return false;
    int cid = query.value(0).toInt();

    // 3. 插入成绩到 score 表
    query.prepare("INSERT INTO score(student_id, course_id, score) VALUES(?,?,?)");
    query.addBindValue(stu.id);
    query.addBindValue(cid);
    query.addBindValue(stu.score);
    return query.exec();
}

bool StudentManager::deleteStudent(const QString& id)
{
    if(!m_isConnected) return false;
    QSqlQuery query;

    // 先删成绩
    query.prepare("DELETE FROM score WHERE student_id=?");
    query.addBindValue(id);
    query.exec();

    // 再删学生
    query.prepare("DELETE FROM student WHERE id=?");
    query.addBindValue(id);
    return query.exec();
}

bool StudentManager::updateStudent(const Student& stu)
{
    if(!m_isConnected) return false;
    QSqlQuery query;

    // 1. 更新 student 表
    query.prepare("UPDATE student SET name=?, class=? WHERE id=?");
    query.addBindValue(stu.name);
    query.addBindValue(stu.className);
    query.addBindValue(stu.id);
    if(!query.exec()) return false;

    // 2. 获取课程ID
    query.prepare("SELECT course_id FROM course WHERE course_name=?");
    query.addBindValue(stu.course);
    if(!query.exec() || !query.next()) return false;
    int cid = query.value(0).toInt();

    // 3. 更新 score 表
    query.prepare("UPDATE score SET score=? WHERE student_id=? AND course_id=?");
    query.addBindValue(stu.score);
    query.addBindValue(stu.id);
    query.addBindValue(cid);
    return query.exec();
}

QList<Student> StudentManager::searchById(const QString& id)
{
    QList<Student> list;
    QSqlQuery query(R"(
        SELECT s.id,s.name,s.class,c.course_name,sc.score
        FROM student s LEFT JOIN score sc ON s.id=sc.student_id
        LEFT JOIN course c ON sc.course_id=c.course_id WHERE s.id=?
    )");
    query.addBindValue(id);
    query.exec();
    while(query.next())
        list.append(Student(query.value(0).toString(),query.value(1).toString(),
                            query.value(2).toString(),query.value(3).toString(),query.value(4).toDouble()));
    return list;
}

QList<Student> StudentManager::searchByName(const QString& name)
{
    QList<Student> list;
    QSqlQuery query(R"(
        SELECT s.id,s.name,s.class,c.course_name,sc.score
        FROM student s LEFT JOIN score sc ON s.id=sc.student_id
        LEFT JOIN course c ON sc.course_id=c.course_id WHERE s.name=?
    )");
    query.addBindValue(name);
    query.exec();
    while(query.next())
        list.append(Student(query.value(0).toString(),query.value(1).toString(),
                            query.value(2).toString(),query.value(3).toString(),query.value(4).toDouble()));
    return list;
}

QList<Student> StudentManager::searchByClass(const QString& cls)
{
    QList<Student> list;
    QSqlQuery query(R"(
        SELECT s.id,s.name,s.class,c.course_name,sc.score
        FROM student s LEFT JOIN score sc ON s.id=sc.student_id
        LEFT JOIN course c ON sc.course_id=c.course_id WHERE s.class LIKE ?
    )");
    query.addBindValue("%"+cls+"%");
    query.exec();
    while(query.next())
        list.append(Student(query.value(0).toString(),query.value(1).toString(),
                            query.value(2).toString(),query.value(3).toString(),query.value(4).toDouble()));
    return list;
}

QList<Student> StudentManager::searchByCourse(const QString& course)
{
    QList<Student> list;
    QSqlQuery query(R"(
        SELECT s.id,s.name,s.class,c.course_name,sc.score
        FROM student s LEFT JOIN score sc ON s.id=sc.student_id
        LEFT JOIN course c ON sc.course_id=c.course_id WHERE c.course_name LIKE ?
    )");
    query.addBindValue("%"+course+"%");
    query.exec();
    while(query.next())
        list.append(Student(query.value(0).toString(),query.value(1).toString(),
                            query.value(2).toString(),query.value(3).toString(),query.value(4).toDouble()));
    return list;
}

QMap<QString, double> StudentManager::statistics(const QString& course)
{
    QMap<QString, double> res;
    res["avg"]=res["max"]=res["min"]=res["pass"]=0;
    QSqlQuery query;
    query.prepare(R"(
        SELECT AVG(sc.score),MAX(sc.score),MIN(sc.score),COUNT(*)
        FROM score sc LEFT JOIN course c ON sc.course_id=c.course_id WHERE c.course_name=?
    )");
    query.addBindValue(course);
    if(!query.exec()||!query.next()) return res;

    double avg=query.value(0).toDouble();
    double maxs=query.value(1).toDouble();
    double mins=query.value(2).toDouble();
    int total=query.value(3).toInt();
    if(total<=0) return res;

    res["avg"]=avg; res["max"]=maxs; res["min"]=mins;
    query.prepare(R"(SELECT COUNT(*) FROM score sc
        LEFT JOIN course c ON sc.course_id=c.course_id
        WHERE c.course_name=? AND sc.score>=60)");
    query.addBindValue(course);
    query.exec();
    int pass=query.next()?query.value(0).toInt():0;
    res["pass"]=(double)pass/total*100;
    return res;
}

QList<Student> StudentManager::sortByScore(bool isAsc)
{
    QList<Student> list=getAllStudents();
    if(isAsc) std::sort(list.begin(),list.end(),[](Student a,Student b){return a.score<b.score;});
    else std::sort(list.begin(),list.end(),[](Student a,Student b){return a.score>b.score;});
    return list;
}