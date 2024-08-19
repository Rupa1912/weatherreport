//
// Created by llyfr on 17.4.23.
//
#include <nlohmann/json.hpp>

struct Weather{
    int id;
    std::string main;
    std::string description;
    std::string icon;
};


struct Wind{
    double speed;
    int degree;
    double gust;
};

struct Temperature{
    double day;
    double min;
    double max;
    double night;
    double eve;
    double morn;
    double feels_like_day;
    double feels_like_night;
    double feels_like_eve;
    double feels_like_morn;
};

struct UnixTime{
    int offset;
    int64_t dt;
    int64_t sunrise;
    int64_t sunset;
    int64_t moonrise;
    int64_t moonset;
    double moon_phase;
};

struct Atmospheric{
    int humidity;
    int pressure;
    int clouds;
    int visibility;
    double uvi;
};

struct HourlyForecast{
    double temperature;
    double feels_like;
    Atmospheric atmospheric;
    Wind wind;
    Weather weather;
};

struct DailyForecast{
    Atmospheric atmospheric;
    Temperature temperature;
    UnixTime time;
    Wind wind;
    Weather weather;
};


/*
 * Class used to parse data from json format to solid data types.
 * */
class Forecast {
private:
    nlohmann::json data;
    std::vector<nlohmann::json> days;

    static Wind mapWind(const nlohmann::json&);
    static Weather mapWeather(const nlohmann::json&);
    static Atmospheric mapAtmospheric(const nlohmann::json&);
    static Temperature mapTemperature(const nlohmann::json&);
    static UnixTime mapUnixTime(const nlohmann::json&);



public:
    explicit Forecast(nlohmann::json data);

    size_t getForecastedDaysCount();

    HourlyForecast getHourlyForecast(unsigned int = 0);
    DailyForecast getDailyForecast(unsigned int = 0);



    Weather getWeather(unsigned int = 0);
};
