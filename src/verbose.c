#include "verbose.h"

bool verbose = false;
bool highly_verbose = false;

void setVerbose(bool value) {
    verbose = value;
}

bool getVerbose() {
    return verbose;
}

void setHighlyVerbose(bool value) {
    highly_verbose = value;
}

bool getHighlyVerbose() {
    return highly_verbose;
}
