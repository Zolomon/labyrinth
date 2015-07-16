#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <fstream>
#include <sstream>
#include <memory>

#include "Level.h"

class Utils
{
public:
    static std::shared_ptr<Level> loadLevel(std::string filename);

private:

};
#endif