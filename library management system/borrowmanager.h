#ifndef BORROWMANAGER_H
#define BORROWMANAGER_H

#include <QList>
#include <QString>
#include <QDate>
#include "borrowrecord.h"

class BorrowManager
{
public:
    BorrowManager();
    
    // 借阅管理
    bool borrowBook(const QString &readerId, const QString &isbn,
                    int borrowDays = 7);
    bool returnBook(const QString &readerId, const QString &isbn);
    
    // 查询功能
    QList<BorrowRecord> getActiveBorrows() const;
    QList<BorrowRecord> getReaderBorrows(const QString &readerId) const;
    QList<BorrowRecord> getActiveReaderBorrows(const QString &readerId) const;
    int getActiveBorrowCount(const QString &readerId) const;
    BorrowRecord* findActiveBorrow(const QString &readerId, const QString &isbn);
    QList<BorrowRecord> getAllBorrows() const;
    
    // 文件操作
    bool loadFromFile(const QString &filename);
    bool saveToFile(const QString &filename) const;
    
private:
    QList<BorrowRecord> m_records;
};

#endif // BORROWMANAGER_H