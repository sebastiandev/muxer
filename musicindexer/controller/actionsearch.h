#ifndef ACTIONSEARCH_H
#define ACTIONSEARCH_H

#include <QString>

#include "mbaction.h"

class ActionSearch : public MBAction
{
    Q_OBJECT

public:

    explicit ActionSearch(const QString &query);

signals:
    void searchFinished(const QStringList&);

public slots:

protected:

    virtual bool prepare(     );
    virtual void runAction(   );
    virtual bool cleanup(     );


    QString _query;
};

#endif // ACTIONSEARCH_H
