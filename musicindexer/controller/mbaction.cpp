#include "mbaction.h"
#include "util/worker.h"

MBAction::MBAction() : _actionProgress(0)
                     , _cancelAction(false)
                     , _actionSucceeded(false)
{
}

MBAction::~MBAction()
{
}

void MBAction::execute()
{
    _worker.reset(new Worker());
    connect(_worker.data(),SIGNAL(started()),this, SLOT(Go()));
    this->moveToThread(_worker.data());
    _worker->start();
}

void MBAction::cancel()
{
    _cancelAction = true;
}

void MBAction::Go()
{
    runAction();
}

void MBAction::addAndEmitProgress(int delta)
{
    _actionProgress = (_actionProgress + delta > 100) ? 100 : _actionProgress + delta;
    Q_EMIT actionProgress(_actionProgress);
}

void MBAction::setAndEmitProgress(int absolute)
{
    _actionProgress = absolute > 100 ? 100 : absolute;
    Q_EMIT actionProgress(_actionProgress);
}
