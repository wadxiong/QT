#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include <QString>
#include <QDate>

class BorrowRecord
{
public:
    BorrowRecord();
    BorrowRecord(const QString &readerId, const QString &isbn,
                 const QDate &borrowDate, const QDate &dueDate);
    
    // Getters
    QString getReaderId() const;
    QString getIsbn() const;
    QDate getBorrowDate() const;
    QDate getDueDate() const;
    bool isReturned() const;
    QDate getReturnDate() const;
    double getFine() const;
    
    // Setters
    void setReaderId(const QString &readerId);
    void setIsbn(const QString &isbn);
    void setBorrowDate(const QDate &borrowDate);
    void setDueDate(const QDate &dueDate);
    void setReturned(bool returned);
    void setReturnDate(const QDate &returnDate);
    
    // 计算罚款
    double calculateFine(const QDate &returnDate = QDate::currentDate()) const;
    
    // 序列化
    QString toString() const;
    static BorrowRecord fromString(const QString &str);
    
private:
    QString m_readerId;
    QString m_isbn;
    QDate m_borrowDate;
    QDate m_dueDate;
    bool m_returned;
    QDate m_returnDate;
};

#endif // BORROWRECORD_H