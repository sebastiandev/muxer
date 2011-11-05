#ifndef ICONFIGURATOR_H
#define ICONFIGURATOR_H

#include <QString>

class IConfigurator
{
  public:
    virtual QString GetString( const QString key ) const = 0;
    virtual long long GetLongLong( const QString key ) const = 0;
    virtual int GetInt( const QString key ) const = 0;
    virtual bool GetBool( const QString key, bool defaultValue ) const = 0;

    virtual bool SetInt( const QString key , int value) const = 0;
    virtual bool SetString( const QString key , QString value) const = 0;
};

#endif // ICONFIGURATOR_H
