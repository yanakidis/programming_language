#include "Interpretation.hpp"

void Interpretator::interpretation () {
    pars.analyze ();
    E.execute ( pars.poliz );
}

