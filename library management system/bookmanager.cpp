#include "bookmanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

BookManager::BookManager() {}

bool BookManager::addBook(const Book &book)
{
    // 检查ISBN是否已存在
    if (findBook(book.getIsbn())) {
        qDebug() << "Book with ISBN" << book.getIsbn() << "already exists";
        return false;
    }
    
    m_books.append(book);
    return true;
}

bool BookManager::removeBook(const QString &isbn)
{
    for (int i = 0; i < m_books.size(); ++i) {
        if (m_books[i].getIsbn() == isbn) {
            m_books.removeAt(i);
            return true;
        }
    }
    return false;
}

bool BookManager::updateBook(const Book &book)
{
    for (int i = 0; i < m_books.size(); ++i) {
        if (m_books[i].getIsbn() == book.getIsbn()) {
            m_books[i] = book;
            return true;
        }
    }
    return false;
}

Book* BookManager::findBook(const QString &isbn)
{
    for (int i = 0; i < m_books.size(); ++i) {
        if (m_books[i].getIsbn() == isbn) {
            return &m_books[i];
        }
    }
    return nullptr;
}

QList<Book> BookManager::searchByTitle(const QString &title) const
{
    QList<Book> results;
    for (const Book &book : m_books) {
        if (book.getTitle().contains(title, Qt::CaseInsensitive)) {
            results.append(book);
        }
    }
    return results;
}

QList<Book> BookManager::searchByAuthor(const QString &author) const
{
    QList<Book> results;
    for (const Book &book : m_books) {
        if (book.getAuthor().contains(author, Qt::CaseInsensitive)) {
            results.append(book);
        }
    }
    return results;
}

QList<Book> BookManager::searchByIsbn(const QString &isbn) const
{
    QList<Book> results;
    for (const Book &book : m_books) {
        if (book.getIsbn().contains(isbn, Qt::CaseInsensitive)) {
            results.append(book);
        }
    }
    return results;
}

QList<Book> BookManager::getAllBooks() const
{
    return m_books;
}

bool BookManager::loadFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for reading:" << filename;
        return false;
    }
    
    QTextStream in(&file);
    m_books.clear();
    
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            m_books.append(Book::fromString(line));
        }
    }
    
    file.close();
    qDebug() << "Loaded" << m_books.size() << "books from" << filename;
    return true;
}

bool BookManager::saveToFile(const QString &filename) const
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing:" << filename;
        return false;
    }
    
    QTextStream out(&file);
    for (const Book &book : m_books) {
        out << book.toString() << "\n";
    }
    
    file.close();
    qDebug() << "Saved" << m_books.size() << "books to" << filename;
    return true;
}