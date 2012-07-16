#ifndef ACTIONSEARCH_H
#define ACTIONSEARCH_H

#include <QString>

#include "mbaction.h"

class ActionSearch : public MBAction
{
    Q_OBJECT

public:

    explicit ActionSearch(const QString &query);

Q_SIGNALS:
    void searchFinished(const QStringList&);

public Q_SLOTS:

protected:

    virtual bool prepare(     );
    virtual void runAction(   );
    virtual bool cleanup(     );


    QString _query;
};

#endif // ACTIONSEARCH_H
