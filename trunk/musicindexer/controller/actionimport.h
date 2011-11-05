#ifndef ACTIONIMPORT_H
#define ACTIONIMPORT_H

#include "mbaction.h"
#include <QStringList>
#include <QDir>

class ActionImport : public MBAction
{
    Q_OBJECT

public:

    ActionImport(QStringList);
    ActionImport(QDir);

signals:

public slots:

protected:

    virtual bool prepare(     );
    virtual void runAction(   );
    virtual bool cleanup(     );

private:

    QStringList _filesToImport;
    QDir        _dirToImport;
};

#endif // ACTIONIMPORT_H
