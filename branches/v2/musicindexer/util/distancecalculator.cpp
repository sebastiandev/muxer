#include "distancecalculator.h"
#include <vector>

int DistanceCalculator::distance(const QString &source, const QString &target)
{
    // Step 1
    const int n = source.length();
    const int m = target.length();

    if (n == 0) return m;
    if (m == 0) return n;

    // Good form to declare a TYPEDEF
    typedef std::vector< std::vector<int> > Tmatrix;

    Tmatrix matrix(n+1);

    // Size the vectors in the 2.nd dimension. Unfortunately C++ doesn't
    // allow for allocation on declaration of 2.nd dimension of vec of vec
    for (int i = 0; i <= n; i++)
      matrix[i].resize(m+1);

    // Step 2
    for (int i = 0; i <= n; i++)
      matrix[i][0]=i;

    for (int j = 0; j <= m; j++)
      matrix[0][j]=j;

    // Step 3
    for (int i = 1; i <= n; i++)
    {
        const char s_i = source.at(i-1).toAscii();

        // Step 4
        for (int j = 1; j <= m; j++)
        {
            const char t_j = target.at(j-1).toAscii();

            // Step 5
            int cost = (s_i == t_j) ? 0 : 1;

            // Step 6
            int above = matrix[i-1][j];
            int left  = matrix[i][j-1];
            int diag  = matrix[i-1][j-1];
            int cell  = min( above + 1, min(left + 1, diag + cost));

            // Step 6A: Cover transposition, in addition to deletion,
            // insertion and substitution. This step is taken from:
            // Berghel, Hal ; Roach, David : "An Extension of Ukkonen's
            // Enhanced Dynamic Programming ASM Algorithm"
            // (http://www.acm.org/~hlb/publications/asm/asm.html)
            if (i>2 && j>2)
            {
                int trans = matrix[i-2][j-2]+1;

                if (source.at(i-2).toAscii() != t_j) trans++;
                if (target.at(j-2).toAscii() != s_i) trans++;
                if (cell > trans) cell = trans;
            }

            matrix[i][j] = cell;
        }
    }

    // Step 7
    return matrix[n][m];
}


int DistanceCalculator::min(int a, int b)
{
    return (a < b ? a : b);
}
