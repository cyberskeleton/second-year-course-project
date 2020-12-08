#include <fstream>
#include <sstream>
#include "../headers/service.h"

std::vector<std::vector<std::string>> service::parseCSV(std::string line, const std::string& fileName) {
    std::vector<std::vector<std::string>> parsedCsv;
    std::ifstream data(fileName);
    while(std::getline(data,line)) {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<std::string> parsedRow;
        while(std::getline(lineStream,cell,',')) {
            parsedRow.push_back(cell);
        }
        parsedCsv.push_back(parsedRow);
    }

    return parsedCsv;
}
