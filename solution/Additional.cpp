#include "Additional.hpp"

vector <Ident> TID;
vector <string> STR_TABLE;

int put (const string & buf) {
    vector<Ident>::iterator k;
    if ( ( k = find ( TID.begin (), TID.end (), buf ) ) != TID.end () ) {
        return k - TID.begin();;
    }
    TID.push_back ( Ident(buf) );
    return TID.size () - 1;
}
