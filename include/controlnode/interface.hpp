#pragma once

#include <curl/curl.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace asio = boost::asio;
namespace http = boost::beast::http;
using tcp = asio::ip::tcp;

static size_t write_callback(void* contents, size_t size, size_t nmemb,
                             std::string* data);
std::string fetch_overpass_data(const std::string& query);
std::string read_file(const std::string& file_path);
void handle_request(tcp::socket& socket);