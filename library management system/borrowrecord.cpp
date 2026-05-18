#include "borrowrecord.h"

BorrowRecord::BorrowRecord() : m_returned(false) {}

BorrowRecord::BorrowRecord(const QString &readerId, const QString &isbn,
                         const QDate &borrowDate, const QDate &dueDate)
    : m_readerId(readerId), m_isbn(isbn), m_borrowDate(borrowDate),
      m_dueDate(dueDate), m_returned(false) {}

QString BorrowRecord::getReaderId() const { return m_readerId; }
QString BorrowRecord::getIsbn() const { return m_isbn; }
QDate BorrowRecord::getBorrowDate() const { return m_borrowDate; }
QDate BorrowRecord::getDueDate() const { return m_dueDate; }
bool BorrowRecord::isReturned() const { return m_returned; }
QDate BorrowRecord::getReturnDate() const { return m_returnDate; }

void BorrowRecord::setReaderId(const QString &readerId) { m_readerId = readerId; }
void BorrowRecord::setIsbn(const QString &isbn) { m_isbn = isbn; }
void BorrowRecord::setBorrowDate(const QDate &borrowDate) { m_borrowDate = borrowDate; }
void BorrowRecord::setDueDate(const QDate &dueDate) { m_dueDate = dueDate; }
void BorrowRecord::setReturned(bool returned) { m_returned = returned; }
void BorrowRecord::setReturnDate(const QDate &returnDate) { m_returnDate = returnDate; }

double BorrowRecord::calculateFine(const QDate &returnDate) const
{
    if (m_returned && m_returnDate <= m_dueDate) {
        return 0.0;
    }
    
    QDate checkDate = m_returned ? m_returnDate : returnDate;
    if (checkDate <= m_dueDate) {
        return 0.0;
    }
    
    int overdueDays = m_dueDate.daysTo(checkDate);
    return overdueDays * 1.0; // 每天1元罚款
}

double BorrowRecord::getFine() const
{
    return calculateFine();
}

QString BorrowRecord::toString() const
{
    return QString("%1|%2|%3|%4|%5|%6")
        .arg(m_readerId, m_isbn,
             m_borrowDate.toString("yyyy-MM-dd"),
             m_dueDate.toString("yyyy-MM-dd"))
        .arg(m_returned ? "1" : "0")
        .arg(m_returned ? m_returnDate.toString("yyyy-MM-dd") : "");
}

BorrowRecord BorrowRecord::fromString(const QString &str)
{
    QStringList parts = str.split('|');
    if (parts.size() >= 5) {
        BorrowRecord record;
        record.setReaderId(parts[0]);
        record.setIsbn(parts[1]);
        record.setBorrowDate(QDate::fromString(parts[2], "yyyy-MM-dd"));
        record.setDueDate(QDate::fromString(parts[3], "yyyy-MM-dd"));
        record.setReturned(parts[4] == "1");
        if (record.isReturned() && parts.size() >= 6) {
            record.setReturnDate(QDate::fromString(parts[5], "yyyy-MM-dd"));
        }
        return record;
    }
    return BorrowRecord();
}