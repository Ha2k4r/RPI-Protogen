#ifndef CONFIG_H
#define CONFIG_H
//Change these values to dramatically change the function of the protogen

// TESTING_ENVIRONMENT and VERBOSE_LOGGING are now set via CMake options.
// See CMakePresets.json or pass -DTESTING_ENVIRONMENT=ON/OFF and -DVERBOSE_LOGGING=ON/OFF.

void init(std::vector<Expression>& faces);

#endif
