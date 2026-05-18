#ifndef READER_H
#define READER_H

#include <QString>

class Reader
{
public:
    Reader();
    Reader(const QString &id, const QString &name, const QString &phone,
           const QString &email);
    
    // Getters
    QString getId() const;
    QString getName() const;
    QString getPhone() const;
    QString getEmail() const;
    
    // Setters
    void setId(const QString &id);
    void setName(const QString &name);
    void setPhone(const QString &phone);
    void setEmail(const QString &email);
    
    // 序列化
    QString toString() const;
    static Reader fromString(const QString &str);
    
private:
    QString m_id;
    QString m_name;
    QString m_phone;
    QString m_email;
};

#endif // READER_H