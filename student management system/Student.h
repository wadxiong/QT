#ifndef STUDENT_H
#define STUDENT_H

#include <QString>

struct Student
{
    QString id;
    QString name;
    QString className;
    QString course;
    double score;

    Student() : score(0.0) {}
    Student(QString id, QString name, QString className, QString course, double score)
        : id(id), name(name), className(className), course(course), score(score) {}
};

#endif // STUDENT_H