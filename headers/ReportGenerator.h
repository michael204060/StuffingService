#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <string>
#include <vector>
#include "../headers/Specialist.h"
#include <sstream>


class ReportGenerator {
public:
    std::string generateXMLReport(const std::vector<Specialist*>& specialists);
};

#endif // REPORTGENERATOR_H