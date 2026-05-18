#include "reader.h"
#include <QStringList>

Reader::Reader() {}

Reader::Reader(const QString &id, const QString &name, const QString &phone,
               const QString &email)
    : m_id(id), m_name(name), m_phone(phone), m_email(email) {}

QString Reader::getId() const { return m_id; }
QString Reader::getName() const { return m_name; }
QString Reader::getPhone() const { return m_phone; }
QString Reader::getEmail() const { return m_email; }

void Reader::setId(const QString &id) { m_id = id; }
void Reader::setName(const QString &name) { m_name = name; }
void Reader::setPhone(const QString &phone) { m_phone = phone; }
void Reader::setEmail(const QString &email) { m_email = email; }

QString Reader::toString() const
{
    return QString("%1|%2|%3|%4")
        .arg(m_id, m_name, m_phone, m_email);
}

Reader Reader::fromString(const QString &str)
{
    QStringList parts = str.split('|');
    if (parts.size() == 4) {
        return Reader(parts[0], parts[1], parts[2], parts[3]);
    }
    return Reader();
}