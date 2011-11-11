#ifndef MEXCEPTION_H
#define MEXCEPTION_H

#include <QtCore>

class MException : public QtConcurrent::Exception
{
 public:

    MException(const QString &msg){_msg = msg;}

    virtual Exception *clone() const { return new MException(*this); }
    void raise() const { throw *this; }

    QString getMsg      () const { return _msg;}
    int     getErrorCode() const { return _code;}

    void    setErrorCode(int code){ _code = code;}

private:

    QString _msg;
    int     _code;
};

#endif // MEXCEPTION_H
