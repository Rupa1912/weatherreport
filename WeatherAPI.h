//
// Created by llyfr on 3/27/23.
//
#pragma once
#include <string>

#include "Forecast.h"
#include "GeoAPI.h"

namespace API {


    namespace UNITS {
        static std::string METRIC = "metric";
        static std::string IMPERIAL = "imperial";
        static std::string STANDARD = "standard";
    }



    class WeatherAPI {
    private:
        std::string key;
        Cords cords;
        std::string units;

    public:
        WeatherAPI() = default;
        explicit WeatherAPI(std::string key);
        WeatherAPI(std::string key, Cords cords, std::string  units = UNITS::METRIC);

        Forecast getForecast();
        void setLocation(Cords);
        Cords getLocation();

    };

}

