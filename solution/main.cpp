#include "Interpretation.hpp"

int main (int argc, char* argv[]) {
    try {
        if (argc > 1) {
            Interpretator I (argv [1]);
            I.interpretation ();
        } else throw "no file name";
        return 0;
    }
    catch (string  s) {
        cout << s << endl;
        return -1;
    }
    catch (Lex l) {
        cout << "Unexpected lexeme" << l << endl;
        return -1;
    }
    catch (const char *s) {
        cout << s << endl;
        return -1;
    }
}
