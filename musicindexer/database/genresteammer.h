#ifndef GENRESTEAMMER_H
#define GENRESTEAMMER_H
#include <QString>

class GenreSteammer
{
public:

    ~GenreSteammer();

    static QString steam(const QString &buffer);

protected:

     GenreSteammer(){}
};

#endif // GENRESTEAMMER_H
