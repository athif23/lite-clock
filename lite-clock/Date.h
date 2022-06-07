#pragma once

const char* HIJRI_MONTH_NAMES[12] = {
    "Muharram",
    "Safar",
    "Rabi' al-'Awwal",
    "Rabi' ath-Thani",
    "Jumada al-'Ula",
    "Jumada ath-'Akhirah",
    "Rajab",
    "Sha'ban",
    "Ramadhan",
    "Shawwal",
    "Dhu al-Qa'dah",
    "Dhu al-Hijjah",
};

const char* GEOGRIAN_MONTH_NAMES[12] = {
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December",
};

const char* DAY_NAMES[7] = {
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday",
        "Sunday",
};

const char* toHMonth(int month) {
    return HIJRI_MONTH_NAMES[month];
}

const char* toGMonth(int month) {
    return GEOGRIAN_MONTH_NAMES[month];
}

const char* toDay(int day) {
    return DAY_NAMES[day];
}

struct DayMonthYear {
    int year;
    int month;
    int day;
};

struct GeogrianDate {
    int year;
    int month;
    int day;
};

struct Clock
{
    time_t rawtime;
    tm timeinfo{};
    char buffer[80] = "00:00:00";
};

struct Date
{
    time_t rawtime;
    tm timeinfo{};
    char buffer[500] = "Monday, 23 September 2001";
};