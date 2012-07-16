#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <QString>

class Exception : public std::exception
{
private:
   QString _error;
public:

   Exception()
   {
       _error.append("Exception");
   }

   Exception(QString error)
   {
       _error.clear();
       _error.append("Exception::");
       _error.append(error);
   }


   ~Exception() throw () {}

   const char* what() const throw()
   {
       return _error.toStdString().c_str();
   }
};

#endif // EXCEPTION_H
