#include "../headers/ReportGenerator.h"
#include <sstream>
#include <iomanip> // Include for formatting average rating


std::string ReportGenerator::generateXMLReport(const std::vector<Specialist*>& specialists) {
    std::stringstream xml;
    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml << "<Specialists>\n";

    for (const Specialist* specialist : specialists) {
        xml << "  <Specialist>\n";
        xml << "    <FirstName>" << specialist->getFirstName() << "</FirstName>\n";
        xml << "    <LastName>" << specialist->getLastName() << "</LastName>\n";
        
        double avgRating = specialist->getAverageRating();
        if (avgRating > 0) {
             xml << "    <Rating>" << std::fixed << std::setprecision(1) << avgRating << "</Rating>\n"; // Formatted rating
        }

        for (const std::string& review : specialist->getReviews()) {
            xml << "    <Review>" << review << "</Review>\n";
        }

        xml << "    <Specialization>" << specialist->getSpecialization() << "</Specialization>\n";
        for (const std::string& certification : specialist->getCertifications()) {
            xml << "    <Certification>" << certification << "</Certification>\n";
        }

         xml << "    <ContactInfo>" << specialist->getContactInfo() << "</ContactInfo>\n";

        xml << "    <Address>\n";
        xml << "      <Country>" << specialist->getAddress().getCountry() << "</Country>\n";
        xml << "      <Region>" << specialist->getAddress().getRegion() << "</Region>\n";
        xml << "      <City>" << specialist->getAddress().getCity() << "</City>\n";
        xml << "      <Street>" << specialist->getAddress().getStreet() << "</Street>\n";
        xml << "      <House>" << specialist->getAddress().getHouse() << "</House>\n";
        xml << "      <Apartment>" << specialist->getAddress().getApartment() << "</Apartment>\n";
        xml << "    </Address>\n";
        xml << "  </Specialist>\n";
    }

    if (specialists.empty()) {
        xml << "  <NoMatch>No matching specialists found.</NoMatch>\n";
    }

    xml << "</Specialists>\n";
    return xml.str();
}