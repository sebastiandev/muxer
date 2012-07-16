#ifndef INDEXEREXCEPTION_H
#define INDEXEREXCEPTION_H

#include "util/MException.h"

class IndexerException : public MException
{
public:

    IndexerException(const QString &msg):MException(msg){}
    IndexerException(const QString &msg, const QString &file):MException(msg){ _file = file;}
    virtual ~IndexerException() throw() {}

    IndexerException *clone() const { return new IndexerException(*this); }

    QString getFile() const { return _file;}

    enum errorCode{ DATABASE_ERROR, EMPTY_TERM};

private:

    QString _file;
};

#endif // INDEXEREXCEPTION_H
