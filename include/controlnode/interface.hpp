#pragma once

#include <curl/curl.h>

#include <iostream>

static size_t write_callback(void* contents, size_t size, size_t nmemb,
                             std::string* data);
std::string fetch_overpass_data(const std::string& query);