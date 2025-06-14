#ifndef CONFIG_H
#define CONFIG_H
//Change these values to dramatically change the function of the protogen

#define TESTING_ENVIRONMENT //Is this on a desktop computer for testing or is this on a raspberry pi that you actually wanna see the RGB matrix


// if this is defined it will compile with debugging if statements which yell at you if you code badly (if you are programming this enable this untill you deploy)
#define NOT_PRODUCTION_ENVIRNMENT

void init(std::vector<Expression>& faces);



#endif
