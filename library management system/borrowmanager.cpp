#include "borrowmanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

BorrowManager::BorrowManager() {}

bool BorrowManager::borrowBook(const QString &readerId, const QString &isbn,
                               int borrowDays)
{
    QDate borrowDate = QDate::currentDate();
    QDate dueDate = borrowDate.addDays(borrowDays);
    
    BorrowRecord record(readerId, isbn, borrowDate, dueDate);
    m_records.append(record);
    return true;
}

bool BorrowManager::returnBook(const QString &readerId, const QString &isbn)
{
    for (int i = 0; i < m_records.size(); ++i) {
        if (m_records[i].getReaderId() == readerId &&
            m_records[i].getIsbn() == isbn &&
            !m_records[i].isReturned()) {
            m_records[i].setReturned(true);
            m_records[i].setReturnDate(QDate::currentDate());
            return true;
        }
    }
    return false;
}

QList<BorrowRecord> BorrowManager::getActiveBorrows() const
{
    QList<BorrowRecord> active;
    for (const BorrowRecord &record : m_records) {
        if (!record.isReturned()) {
            active.append(record);
        }
    }
    return active;
}

QList<BorrowRecord> BorrowManager::getReaderBorrows(const QString &readerId) const
{
    QList<BorrowRecord> readerBorrows;
    for (const BorrowRecord &record : m_records) {
        if (record.getReaderId() == readerId) {
            readerBorrows.append(record);
        }
    }
    return readerBorrows;
}

QList<BorrowRecord> BorrowManager::getActiveReaderBorrows(const QString &readerId) const
{
    QList<BorrowRecord> active;
    for (const BorrowRecord &record : m_records) {
        if (record.getReaderId() == readerId && !record.isReturned()) {
            active.append(record);
        }
    }
    return active;
}

int BorrowManager::getActiveBorrowCount(const QString &readerId) const
{
    return getActiveReaderBorrows(readerId).size();
}

BorrowRecord* BorrowManager::findActiveBorrow(const QString &readerId, const QString &isbn)
{
    for (int i = 0; i < m_records.size(); ++i) {
        if (m_records[i].getReaderId() == readerId &&
            m_records[i].getIsbn() == isbn &&
            !m_records[i].isReturned()) {
            return &m_records[i];
        }
    }
    return nullptr;
}

QList<BorrowRecord> BorrowManager::getAllBorrows() const
{
    return m_records;
}

bool BorrowManager::loadFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for reading:" << filename;
        return false;
    }
    
    QTextStream in(&file);
    m_records.clear();
    
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            m_records.append(BorrowRecord::fromString(line));
        }
    }
    
    file.close();
    qDebug() << "Loaded" << m_records.size() << "borrow records from" << filename;
    return true;
}

bool BorrowManager::saveToFile(const QString &filename) const
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing:" << filename;
        return false;
    }
    
    QTextStream out(&file);
    for (const BorrowRecord &record : m_records) {
        out << record.toString() << "\n";
    }
    
    file.close();
    qDebug() << "Saved" << m_records.size() << "borrow records to" << filename;
    return true;
}