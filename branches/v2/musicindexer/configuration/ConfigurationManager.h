#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include <QString>
#include "IConfigurator.h"

class IConfigurator;
class ConfigurationManager
{
  public:
      static void SetConfigurator(IConfigurator * configurator);

      static QString GetString( const QString key );
      static long long GetLongLong( const QString key );
      static int GetInt( const QString key );
      static bool GetBool( const QString key, bool defaultValue );

      static bool SetInt( const QString key, int value);
      static bool SetString( const QString key, QString value);

  private:
      static IConfigurator *_configurator;
};

#endif // CONFIGURATION_MANAGER_H
