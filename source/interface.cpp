#include <controlnode/interface.hpp>

static size_t write_callback(void* contents, size_t size, size_t nmemb,
                             std::string* data) {
  data->append((char*)contents, size * nmemb);
  return size * nmemb;
}

std::string fetch_overpass_data(const std::string& query) {
  CURL* curl = curl_easy_init();
  if (!curl) throw std::runtime_error("Faild to initialize CURL");

  std::string response;
  curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    curl_easy_cleanup(curl);
    throw std::runtime_error("CURL error: " +
                             std::string(curl_easy_strerror(res)));
  }
  curl_easy_cleanup(curl);
  return response;
}
