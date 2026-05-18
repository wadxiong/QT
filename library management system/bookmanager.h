#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H

#include <QList>
#include <QString>
#include "book.h"

class BookManager
{
public:
    BookManager();
    
    // 图书管理
    bool addBook(const Book &book);
    bool removeBook(const QString &isbn);
    bool updateBook(const Book &book);
    Book* findBook(const QString &isbn);
    
    // 搜索功能
    QList<Book> searchByTitle(const QString &title) const;
    QList<Book> searchByAuthor(const QString &author) const;
    QList<Book> searchByIsbn(const QString &isbn) const;
    QList<Book> getAllBooks() const;
    
    // 文件操作
    bool loadFromFile(const QString &filename);
    bool saveToFile(const QString &filename) const;
    
private:
    QList<Book> m_books;
};

#endif // BOOKMANAGER_H