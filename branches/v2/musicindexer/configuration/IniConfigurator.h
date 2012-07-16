#ifndef INI_CONFIGURATOR_H
#define INI_CONFIGURATOR_H

#include "IConfigurator.h"

class IniConfigurator : public IConfigurator
{
  public:

    IniConfigurator(){}
    IniConfigurator(const QString confPath);

    virtual QString GetString( const QString key ) const;
    virtual long long GetLongLong( const QString key ) const;
    virtual int GetInt( const QString key ) const;
    virtual bool GetBool( const QString key, bool defaultValue ) const;
    virtual bool SetInt( const QString key, int value ) const;
    virtual bool SetString(const QString key, QString value ) const;

private:
    QString _confPath, _confFile;
};

#endif // INI_CONFIGURATOR_H
