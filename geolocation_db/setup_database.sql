create database geolocations;

\c geolocations;

create table if not exists geolocation_hots (
    geolocation_hosts_id serial primary key,
    host text unique,
    ip varchar(39),
    type varchar(4),
    continent_code varchar(3),
    continent_name varchar(32),
    country_code varchar(3),
    country_name varchar(32),
    region_code varchar(3),
    region_name varchar(32),
    city varchar(32),
    zip varchar(6),
    latitude  real not null,
    longitude real not null
)
