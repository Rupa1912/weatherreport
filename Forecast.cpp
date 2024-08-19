//
// Created by llyfr on 17.4.23.
//

#include "Forecast.h"
#include <utility>
#include "iostream"

Forecast::Forecast(nlohmann::json data): data(std::move(data)) {

    for(const auto& day : this->data.at("daily")){
        this->days.push_back(day);
    }

}

size_t Forecast::getForecastedDaysCount() {
    return this->days.size();
}


/**
 * Returns weather for current day on specified hour.
 * If n is zero will return current weather.
 * @param n
 * @return HourlyForecast
 */
HourlyForecast Forecast::getHourlyForecast(unsigned int n) {
    HourlyForecast hourlyWeather;

    nlohmann::json hourlyWeatherJson;

    if(n == 0)
        hourlyWeatherJson = this->data.at("current");
    else
        hourlyWeatherJson = this->data.at("hourly")[n];


    hourlyWeather.temperature = hourlyWeatherJson.value("temp", 0.0);
    hourlyWeather.feels_like = hourlyWeatherJson.value("feels_like", 0.0);

    hourlyWeather.atmospheric = mapAtmospheric(hourlyWeatherJson);
    hourlyWeather.wind = mapWind(hourlyWeatherJson);
    hourlyWeather.weather = mapWeather(hourlyWeatherJson.at("weather")[0]);

    return hourlyWeather;
}

DailyForecast Forecast::getDailyForecast(unsigned int n) {
    DailyForecast dailyForecast;

    dailyForecast.atmospheric = mapAtmospheric(this->days[n]);
    dailyForecast.weather = mapWeather(this->days[n].at("weather")[0]);
    dailyForecast.wind = mapWind(this->days[n]);
    dailyForecast.time = mapUnixTime(this->days[n]);
    dailyForecast.time.offset = this->data.value("timezone_offset", 0);
    dailyForecast.temperature = mapTemperature(this->days[n]);


    return dailyForecast;
}


Weather Forecast::mapWeather(const nlohmann::json& weather_json) {

    Weather weather;

    weather.id = weather_json.value("id", 0);
    weather.main = weather_json.value("main", "");
    weather.description = weather_json.value("description", "");

    // The API allows you to specify the size of the icon, but it only seems to support only 2x and 4x and I will always need 4x.
    weather.icon = "https://openweathermap.org/img/wn/" + weather_json.value("icon","01d") + "@4x.png";

    return weather;

}


Wind Forecast::mapWind(const nlohmann::json& wind_json) {
    Wind wind{};

    wind.gust = wind_json.value("wind_gust", 0.0);
    wind.speed = wind_json.value("wind_speed", 0.0);
    wind.degree = wind_json.value("wind_deg", 0);

    return wind;
}

Atmospheric Forecast::mapAtmospheric(const nlohmann::json& atmospheric_json) {
    Atmospheric atmospheric{};

    atmospheric.pressure = atmospheric_json.value("pressure", 0);
    atmospheric.humidity = atmospheric_json.value("humidity", 0);
    atmospheric.visibility = atmospheric_json.value("visibility", 0);
    atmospheric.clouds = atmospheric_json.value("clouds", 0);
    atmospheric.uvi = atmospheric_json.value("uvi", 0.0);

    return atmospheric;
}

Temperature Forecast::mapTemperature(const nlohmann::json& temperature_json) {
    Temperature temperature{};

    temperature.day = temperature_json.at("temp").value("day", 0.0);
    temperature.min = temperature_json.at("temp").value("min", 0.0);
    temperature.max = temperature_json.at("temp").value("max", 0.0);
    temperature.night = temperature_json.at("temp").value("night", 0.0);
    temperature.eve = temperature_json.at("temp").value("eve", 0.0);

    temperature.feels_like_day = temperature_json.at("feels_like").value("day", 0.0);
    temperature.feels_like_night = temperature_json.at("feels_like").value("night", 0.0);
    temperature.feels_like_eve = temperature_json.at("feels_like").value("eve", 0.0);
    temperature.feels_like_morn = temperature_json.at("feels_like").value("morn", 0.0);

    return temperature;
}

UnixTime Forecast::mapUnixTime(const nlohmann::json& unixTime_json) {
    UnixTime unixTime{};

    unixTime.sunrise = unixTime_json.value("sunrise", 0);
    unixTime.sunset = unixTime_json.value("sunset", 0);
    unixTime.dt = unixTime_json.value("dt", 0);
    unixTime.moonrise = unixTime_json.value("moonrise", 0);
    unixTime.moonset = unixTime_json.value("moonset", 0);
    unixTime.moon_phase = unixTime_json.value("moon_phase", 0.0);

    return unixTime;
}










/*
 *
 *
lat Geographical coordinates of the cords (latitude)
lon Geographical coordinates of the cords (longitude)
timezone Timezone name for the requested cords
timezone_offset Shift in seconds from UTC
current Current weather data API response
current.dt Current time, Unix, UTC
current.sunrise Sunrise time, Unix, UTC
current.sunset Sunset time, Unix, UTC
current.temp Temperature. Units - default: kelvin, metric: Celsius, imperial: Fahrenheit. How to change units used
current.feels_like Temperature. This temperature parameter accounts for the human perception of weather. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
current.pressure Atmospheric pressure on the sea level, hPa
current.humidity Humidity, %
current.dew_point Atmospheric temperature (varying according to pressure and humidity) below which water droplets begin to condense and dew can form. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
current.clouds Cloudiness, %
current.uvi Current UV index
current.visibility Average visibility, metres. The maximum value of the visibility is 10km
current.wind_speed Wind speed. Wind speed. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour. How to change units used
current.wind_gust (where available) Wind gust. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour. How to change units used
current.wind_deg Wind direction, degrees (meteorological)
current.rain
current.rain.1h (where available) Precipitation, mm/h
current.snow
current.snow.1h (where available) Precipitation, mm/h
current.weather
current.weather.id Weather condition id
current.weather.main Group of weather parameters (Rain, Snow, Extreme etc.)
current.weather.description Weather condition within the group (full list of weather conditions). Get the output in your language
current.weather.icon Weather icon id. How to get icons
minutely Minute forecast weather data API response
minutely.dt Time of the forecasted data, unix, UTC
minutely.precipitation Precipitation, mm/h
hourly Hourly forecast weather data API response
hourly.dt Time of the forecasted data, Unix, UTC
hourly.temp Temperature. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit. How to change units used
hourly.feels_like Temperature. This accounts for the human perception of weather. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
hourly.pressure Atmospheric pressure on the sea level, hPa
hourly.humidity Humidity, %
hourly.dew_point Atmospheric temperature (varying according to pressure and humidity) below which water droplets begin to condense and dew can form. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
hourly.uvi UV index
hourly.clouds Cloudiness, %
hourly.visibility Average visibility, metres. The maximum value of the visibility is 10km
hourly.wind_speed Wind speed. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour.How to change units used
hourly.wind_gust (where available) Wind gust. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour. How to change units used
chourly.wind_deg Wind direction, degrees (meteorological)
hourly.pop Probability of precipitation. The values of the parameter vary between 0 and 1, where 0 is equal to 0%, 1 is equal to 100%
hourly.rain
hourly.rain.1h (where available) Precipitation, mm/h
hourly.snow
hourly.snow.1h (where available) Precipitation, mm/h
hourly.weather
hourly.weather.id Weather condition id
hourly.weather.main Group of weather parameters (Rain, Snow, Extreme etc.)
hourly.weather.description Weather condition within the group (full list of weather conditions). Get the output in your language
hourly.weather.icon Weather icon id. How to get icons
daily Daily forecast weather data API response
daily.dt Time of the forecasted data, Unix, UTC
daily.sunrise Sunrise time, Unix, UTC
daily.sunset Sunset time, Unix, UTC
daily.moonrise The time of when the moon rises for this day, Unix, UTC
daily.moonset The time of when the moon sets for this day, Unix, UTC
daily.moon_phase Moon phase. 0 and 1 are 'new moon', 0.25 is 'first quarter moon', 0.5 is 'full moon' and 0.75 is 'last quarter moon'. The periods in between are called 'waxing crescent', 'waxing gibous', 'waning gibous', and 'waning crescent', respectively.
daily.temp Units – default: kelvin, metric: Celsius, imperial: Fahrenheit. How to change units used
daily.temp.morn Morning temperature.
daily.temp.day Day temperature.
daily.temp.eve Evening temperature.
daily.temp.night Night temperature.
daily.temp.min Min daily temperature.
daily.temp.max Max daily temperature.
daily.feels_like This accounts for the human perception of weather. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit. How to change units used
daily.feels_like.morn Morning temperature.
daily.feels_like.day Day temperature.
daily.feels_like.eve Evening temperature.
daily.feels_like.night Night temperature.
daily.pressure Atmospheric pressure on the sea level, hPa
daily.humidity Humidity, %
daily.dew_point Atmospheric temperature (varying according to pressure and humidity) below which water droplets begin to condense and dew can form. Units – default: kelvin, metric: Celsius, imperial: Fahrenheit.
daily.wind_speed Wind speed. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour. How to change units used
daily.wind_gust (where available) Wind gust. Units – default: metre/sec, metric: metre/sec, imperial: miles/hour. How to change units used
daily.wind_deg Wind direction, degrees (meteorological)
daily.clouds Cloudiness, %
daily.uvi The maximum value of UV index for the day
daily.pop Probability of precipitation. The values of the parameter vary between 0 and 1, where 0 is equal to 0%, 1 is equal to 100%
daily.rain (where available) Precipitation volume, mm
daily.snow (where available) Snow volume, mm
daily.weather
daily.weather.id Weather condition id
daily.weather.main Group of weather parameters (Rain, Snow, Extreme etc.)
daily.weather.description Weather condition within the group (full list of weather conditions). Get the output in your language
daily.weather.icon Weather icon id. How to get icons
alerts National weather alerts data from major national weather warning systems
alerts.sender_name Name of the alert source. Please read here the full list of alert sources
alerts.event Alert event name
alerts.start Date and time of the start of the alert, Unix, UTC
alerts.end Date and time of the end of the alert, Unix, UTC
alerts.description Description of the alert
alerts.tags Type of severe weather
 *
 * */