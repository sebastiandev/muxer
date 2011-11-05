#include <QString>
#include <QSettings>
#include <QDebug>
#include <QDir>
#include "IniConfigurator.h"

#define CONFIG_PATH "../resources/"
#define CONFIG_FILE "../resources/config.conf"

IniConfigurator::IniConfigurator(const QString configFile)
{
    _confFile = configFile;
    qDebug() << QDir::currentPath();
}

QString IniConfigurator::GetString( const QString key ) const
{
  QSettings settings(_confFile.isEmpty() ? CONFIG_FILE : _confFile, QSettings::IniFormat);

  if(settings.status() != QSettings::NoError)
      return "INVALID KEY NAME";

  QVariant value = settings.value("musicindexer/" + key).toString();
  if(value.isValid())
    return value.toString();
  return "INVALID KEY NAME";
}

long long IniConfigurator::GetLongLong( const QString key ) const
{
    QSettings settings(_confFile.isEmpty() ? CONFIG_FILE : _confFile, QSettings::IniFormat);

    if(settings.status() != QSettings::NoError)
        return -1;

    QVariant value = settings.value("musicindexer/"  + key).toString();
    if(value.isValid() && value.canConvert(QVariant::LongLong))
        return value.toLongLong();
    return -1;
}

int IniConfigurator::GetInt( const QString key ) const
{
    QSettings settings(_confFile.isEmpty() ? CONFIG_FILE : _confFile, QSettings::IniFormat);

    if(settings.status() != QSettings::NoError)
        return -1;

    QVariant value = settings.value("musicindexer/" + key).toString();

    if(value.isValid() && value.canConvert(QVariant::Int))
       return value.toInt();
    return -1;
}

bool IniConfigurator::GetBool( const QString key, bool defaultValue ) const
{
    QSettings settings(_confFile.isEmpty() ? CONFIG_FILE : _confFile, QSettings::IniFormat);

    if(settings.status() != QSettings::NoError)
        return defaultValue;

    QVariant value = settings.value("musicindexer/" + key).toString();

    if(value.isValid() && value.canConvert(QVariant::Bool))
       return value.toBool();

    return defaultValue;
}

bool IniConfigurator::SetInt( const QString key, int value ) const
{
    QSettings settings(_confFile.isEmpty() ? CONFIG_FILE : _confFile, QSettings::IniFormat);

    if(settings.status() != QSettings::NoError)
        return false;

    settings.setValue("musicindexer/" + key, value);
    return true;
}

bool IniConfigurator::SetString( const QString key, QString value ) const
{
    QSettings settings(_confFile.isEmpty() ? CONFIG_FILE : _confFile, QSettings::IniFormat);

    if(settings.status() != QSettings::NoError)
        return false;

    settings.setValue("musicindexer/" + key, value);
    return true;
}
