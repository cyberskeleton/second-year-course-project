#ifndef INTERVALS_SERVICE_H
#define INTERVALS_SERVICE_H

#include <ostream>
#include <vector>

class service {
private:


public:
    static std::vector<std::vector<std::string>> parseCSV(std::string line, const std::string& fileName);
};


#endif //INTERVALS_SERVICE_H
