#ifndef READERMANAGER_H
#define READERMANAGER_H

#include <QList>
#include <QString>
#include "reader.h"

class ReaderManager
{
public:
    ReaderManager();
    
    // 读者管理
    bool addReader(const Reader &reader);
    bool removeReader(const QString &id);
    bool updateReader(const Reader &reader);
    Reader* findReader(const QString &id);
    QList<Reader> getAllReaders() const;
    
    // 文件操作
    bool loadFromFile(const QString &filename);
    bool saveToFile(const QString &filename) const;
    
private:
    QList<Reader> m_readers;
};

#endif // READERMANAGER_H