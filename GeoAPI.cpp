//
// Created by llyfr on 17.4.23.
//

#include "GeoAPI.h"

#include <utility>
#include <fstream>


API::GeoAPI::GeoAPI(std::string key) :key(std::move(key)) {}

// Thank you https://stackoverflow.com/questions/154536/encode-decode-urls-in-c
std::string url_encode(const std::string &value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (auto i = value.begin(), n = value.end(); i != n; ++i) {
        auto c = *i;

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char)c);
        escaped << std::nouppercase;
    }

    return escaped.str();
}







std::vector<Location> API::GeoAPI::getLocations(const std::string& searchedLocation, int limit) {

    std::vector<Location> locations;

    //std::replace(searchedLocation.begin(),searchedLocation.end(), ' ', '-');

    std::string url = "https://api.openweathermap.org/geo/1.0/direct?q=" + url_encode(searchedLocation) + "&limit=" + std::to_string(limit) + "&appid=" + this->key;

    std::cout << url << std::endl;

    auto data = API::makeAPIcall(url);

    for(auto location : data) {
        Location loc;
        loc.cords.lat = location.at("lat").get<double>();
        loc.cords.lon = location.at("lon").get<double>();
        loc.country = location.at("country").get<std::string>();
        loc.name = location.at("name").get<std::string>();
        loc.state = location.value("state", "N/A");

        locations.push_back(loc);
    }

    return locations;
}



std::vector<Location> API::GeoAPI::getLocations(Cords cords, int limit) {

    std::vector<Location> locations;


    std::string url = "https://api.openweathermap.org/geo/1.0/reverse?lat=" + std::to_string(cords.lat) + "&lon=" +
                              std::to_string(cords.lon) + "&limit=" + std::to_string(limit) + "&appid=" + this->key;


    std::cout << url << std::endl;

    auto data = API::makeAPIcall(url);

    for(auto location : data) {
        Location loc;
        loc.cords.lat = location.at("lat").get<double>();
        loc.cords.lon = location.at("lon").get<double>();
        loc.country = location.at("country").get<std::string>();
        loc.name = location.at("name").get<std::string>();
        loc.state = location.value("state", "N/A");

        locations.push_back(loc);
    }


    return locations;
}


bool API::GeoAPI::testAPIkey() {

    auto data = makeAPIcall("https://api.openweathermap.org/geo/1.0/direct?q=London&appid=" + this->key);

    if(data.is_array()) {
        return true;
    }

    if (data.value("cod", 0) != 401)
        return true;

    return false;
}




nlohmann::json API::makeAPIcall(const std::string& url) {

    std::ostringstream os;
    os << curlpp::options::Url(url);

    std::string string = os.str();

    auto data = nlohmann::json::parse(string);

    return data;

}

std::stringstream API::getImageFromUrl(const std::string &url) {

    std::stringstream image_stream;

    curlpp::Cleanup cleaner;
    curlpp::Easy request;

    request.setOpt(new curlpp::options::Url(url));
    request.setOpt(new curlpp::options::WriteStream(&image_stream));

    request.perform();

    return image_stream;
}
