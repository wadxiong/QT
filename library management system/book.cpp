#include "book.h"
#include <QStringList>

Book::Book() : m_year(2024), m_stock(0) {}

Book::Book(const QString &title, const QString &author, const QString &isbn,
           int year, int stock)
    : m_title(title), m_author(author), m_isbn(isbn), m_year(year), m_stock(stock) {}

QString Book::getTitle() const { return m_title; }
QString Book::getAuthor() const { return m_author; }
QString Book::getIsbn() const { return m_isbn; }
int Book::getYear() const { return m_year; }
int Book::getStock() const { return m_stock; }

void Book::setTitle(const QString &title) { m_title = title; }
void Book::setAuthor(const QString &author) { m_author = author; }
void Book::setIsbn(const QString &isbn) { m_isbn = isbn; }
void Book::setYear(int year) { m_year = year; }
void Book::setStock(int stock) { m_stock = stock; }

void Book::decreaseStock()
{
    if (m_stock > 0) {
        m_stock--;
    }
}

void Book::increaseStock()
{
    m_stock++;
}

QString Book::toString() const
{
    return QString("%1|%2|%3|%4|%5")
        .arg(m_title, m_author, m_isbn)
        .arg(m_year)
        .arg(m_stock);
}

Book Book::fromString(const QString &str)
{
    QStringList parts = str.split('|');
    if (parts.size() == 5) {
        return Book(parts[0], parts[1], parts[2],
                   parts[3].toInt(), parts[4].toInt());
    }
    return Book();
}