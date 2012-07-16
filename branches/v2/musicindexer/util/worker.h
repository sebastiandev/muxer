#ifndef WORKER_H
#define WORKER_H

#include <QThread>

class Worker : public QThread
{
public:

    Worker() {
        setTerminationEnabled(true);
    }

    static void mSleep(unsigned long msecs) { QThread::msleep(msecs); }

protected:
    void run() { exec(); }
};

#endif // WORKER_H
