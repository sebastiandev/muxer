#include "ConfigurationManager.h"
#include "IConfigurator.h"

IConfigurator* ConfigurationManager::_configurator = 0;

void ConfigurationManager::SetConfigurator(IConfigurator * configurator)
{
  _configurator = configurator;
}

QString ConfigurationManager::GetString(const QString key)
{
  return _configurator->GetString(key);
}

long long ConfigurationManager::GetLongLong(const QString key)
{
  return _configurator->GetLongLong(key);
}

int ConfigurationManager::GetInt(const QString key)
{
  return _configurator->GetInt(key);
}

bool ConfigurationManager::GetBool(const QString key, bool defaultValue )
{
  return _configurator->GetBool(key, defaultValue);
}

bool ConfigurationManager::SetInt( const QString key, int value)
{
    return _configurator->SetInt(key,value);
}

bool ConfigurationManager::SetString(const QString key, QString value)
{
    return _configurator->SetString(key,value);
}
