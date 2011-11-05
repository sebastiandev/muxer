#include "genresteammer.h"

QString GenreSteammer::steam(const QString &buffer)
{
    QString result = buffer.toLower();

    result.replace(" "    , "_");
    result.replace("&"    , "_");
    result.replace("'"    , "_");
    result.replace("-"    , "_");
    result.replace("'n"   , "_");
    result.replace(" n "  , "_");
    result.replace("_n_"  , "_");
    result.replace(" and ", "_");
    result.replace("_and_", "_");
    result.replace(" y "  , "_");
    result.replace("_y_"  , "_");

    int underscore = result.indexOf("_");
    if (underscore != -1 && (underscore+1) < result.size()-1)
    {
        // remove consecutive '_' leaving only one        
        while (result.at(underscore+1) == '_')
            result.remove(underscore+1, 1);
    }

    result.replace("á", "a");
    result.replace("é", "e");
    result.replace("í", "i");
    result.replace("ó", "o");
    result.replace("ú", "u");

    // if there is '_' at the end, clean it
    if (result.at(result.size()-1) == '_')
        result.remove(result.size()-1, 1);

    return result;
}
