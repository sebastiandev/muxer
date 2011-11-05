#ifndef DISTANCECALCULATOR_H
#define DISTANCECALCULATOR_H
#include <QString>

class DistanceCalculator
{
public:

    static int distance(const QString &source, const QString &target);

private:

    static int min(int a, int b);
};

#endif // DISTANCECALCULATOR_H
