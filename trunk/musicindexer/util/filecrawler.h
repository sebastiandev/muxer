#ifndef FILECRAWLER_H
#define FILECRAWLER_H

#include <QObject>

class FileCrawler : public QObject
{
    Q_OBJECT
public:
    explicit FileCrawler(QObject *parent = 0);

    void crawlDirectory(const QString &dir);

signals:

    void foundFile(const QString &);

public slots:

};

#endif // FILECRAWLER_H
