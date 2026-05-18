#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include "Student.h"
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>

class StudentManager
{
public:
    static StudentManager* getInstance();
    static void releaseInstance();

    bool connectDB();
    void closeDB();
    bool isDBConnectOk();

    bool addStudent(const Student& stu);
    bool deleteStudent(const QString& id);
    bool updateStudent(const Student& stu);
    QList<Student> getAllStudents();

    QList<Student> searchById(const QString& id);
    QList<Student> searchByName(const QString& name);
    QList<Student> searchByClass(const QString& cls);
    QList<Student> searchByCourse(const QString& course);

    QMap<QString, double> statistics(const QString& course);
    QList<Student> sortByScore(bool isAsc);

private:
    StudentManager();
    ~StudentManager();
    static StudentManager* instance;
    QSqlDatabase db;
    bool m_isConnected;
};

#endif // STUDENTMANAGER_H