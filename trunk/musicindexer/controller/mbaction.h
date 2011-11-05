#ifndef MBACTION_H
#define MBACTION_H

#include <QObject>

class Worker;
class MBAction : public QObject
{
    Q_OBJECT
public:
    MBAction();
    ~MBAction();
    void execute();
    virtual void cancel();

signals:
    void actionStarted();
    void actionProgress(int progress);
    void actionError(const QString & errorDesc);
    void actionEnded(bool succeeded);
    void confirmAction(const QString &, bool *);
    void getInputFromUser(const QString &, QString *);
    void showMessageFile(const QString &, const QString &, int *);

public slots:
    void Go();

protected:
    virtual bool prepare(     ) = 0;
    virtual void runAction(   ) = 0;
    virtual bool cleanup(     ) = 0;
    virtual void addAndEmitProgress(int delta);
    virtual void setAndEmitProgress(int absolute);

protected:
    QScopedPointer<Worker> _worker;
    int    _actionProgress;
    bool    _cancelAction;
    bool    _actionSucceeded;
};

#endif // DBACTION_H
