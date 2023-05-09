#include "verbose.h"

bool verbose = false;

void setVerbose(bool value) {
    verbose = value;
}

bool getVerbose() {
    return verbose;
}
