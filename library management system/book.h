#ifndef BOOK_H
#define BOOK_H

#include <QString>

class Book
{
public:
    Book();
    Book(const QString &title, const QString &author, const QString &isbn,
         int year, int stock = 1);
    
    // Getters
    QString getTitle() const;
    QString getAuthor() const;
    QString getIsbn() const;
    int getYear() const;
    int getStock() const;
    
    // Setters
    void setTitle(const QString &title);
    void setAuthor(const QString &author);
    void setIsbn(const QString &isbn);
    void setYear(int year);
    void setStock(int stock);
    
    // 库存操作
    void decreaseStock();
    void increaseStock();
    
    // 序列化
    QString toString() const;
    static Book fromString(const QString &str);
    
private:
    QString m_title;
    QString m_author;
    QString m_isbn;
    int m_year;
    int m_stock;
};

#endif // BOOK_H