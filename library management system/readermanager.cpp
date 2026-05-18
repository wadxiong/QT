#include "readermanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

ReaderManager::ReaderManager() {}

bool ReaderManager::addReader(const Reader &reader)
{
    if (findReader(reader.getId())) {
        qDebug() << "Reader with ID" << reader.getId() << "already exists";
        return false;
    }
    
    m_readers.append(reader);
    return true;
}

bool ReaderManager::removeReader(const QString &id)
{
    for (int i = 0; i < m_readers.size(); ++i) {
        if (m_readers[i].getId() == id) {
            m_readers.removeAt(i);
            return true;
        }
    }
    return false;
}

bool ReaderManager::updateReader(const Reader &reader)
{
    for (int i = 0; i < m_readers.size(); ++i) {
        if (m_readers[i].getId() == reader.getId()) {
            m_readers[i] = reader;
            return true;
        }
    }
    return false;
}

Reader* ReaderManager::findReader(const QString &id)
{
    for (int i = 0; i < m_readers.size(); ++i) {
        if (m_readers[i].getId() == id) {
            return &m_readers[i];
        }
    }
    return nullptr;
}

QList<Reader> ReaderManager::getAllReaders() const
{
    return m_readers;
}

bool ReaderManager::loadFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for reading:" << filename;
        return false;
    }
    
    QTextStream in(&file);
    m_readers.clear();
    
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            m_readers.append(Reader::fromString(line));
        }
    }
    
    file.close();
    qDebug() << "Loaded" << m_readers.size() << "readers from" << filename;
    return true;
}

bool ReaderManager::saveToFile(const QString &filename) const
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing:" << filename;
        return false;
    }
    
    QTextStream out(&file);
    for (const Reader &reader : m_readers) {
        out << reader.toString() << "\n";
    }
    
    file.close();
    qDebug() << "Saved" << m_readers.size() << "readers to" << filename;
    return true;
}