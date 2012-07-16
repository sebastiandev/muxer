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

Q_SIGNALS:

    void songWithEmptyTerms(const QString&, const QString&);

public Q_SLOTS:

protected:

    virtual bool prepare(     );
    virtual void runAction(   );
    virtual bool cleanup(     );

private Q_SLOTS:

    void slotCalculateProgress(QString file, int count, int total);

private:

    QStringList _filesToImport;
    QDir        _dirToImport;
};

#endif // ACTIONIMPORT_H
