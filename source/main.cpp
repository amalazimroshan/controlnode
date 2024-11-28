#include <curl/curl.h>
#include <math.h>

#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb,
                            std::string* data) {
  data->append((char*)contents, size * nmemb);
  return size * nmemb;
};
std::string fetch_overpass_data(const std::string& query) {
  CURL* curl;
  CURLcode res;
  std::string response;

  curl = curl_easy_init();
  if (!curl) {
    std::cerr << "init failed\n";
    return 0;
  }

  curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    std::cout << query.c_str();
    std::cerr << "fecth issue " << curl_easy_strerror(res) << std::endl;
    return 0;
  }
  curl_easy_cleanup(curl);
  return response;
}
double get_bearing(double lat1, double lon1, double lat2, double lon2) {
  double dy = lat2 - lat1;
  double dx = lon2 - lon1;
  double bearing = std::atan2(dy, dx) * 180.0 / M_PI;
  return std::fmod(bearing + 360.0, 360.0);  // Normalize to 0-360
}

double calculate_distance(double lat1, double lon1, double lat2, double lon2) {
  double dy = lat2 - lat1;
  double dx = lon2 - lon1;
  return sqrt((dx * dx) + (dy * dy));
}
int main() {
  //   std::string overpassUrl = "https://jsonplaceholder.typicode.com/todos/1";
  double lat = 40.741572;
  double lon = -73.989563;
  std::string overpassUrl =
      "https://overpass-api.de/api/"
      "interpreter?data=[out:json];node(around:10," +
      std::to_string(lat) + "," + std::to_string(lon) +
      ");way(bn)[\"highway\"~\"^(motorway|trunk|primary|secondary|tertiary|"
      "unclassified|residential|service|living_street)$\"];"
      "out%20geom;";
  std::string response = fetch_overpass_data(overpassUrl);

  try {
    auto jsonData = json::parse(response);
    std::vector<int> unique_nodes;
    int repeat_node = -1;

    std::unordered_map<int, int> node_count;
    for (const auto& element : jsonData["elements"]) {
      if (element["type"] == "way") {
        for (const auto& node : element["nodes"]) {
          node_count[node]++;
          if (node_count[node] > 1) {
            repeat_node = node;
            break;
          }
        }
        if (repeat_node != -1) break;
      }
    }

    for (const auto& element : jsonData["elements"]) {
      if (element["type"] != "way") continue;
      std::vector<double> bearings;
      std::vector<double> weights;

      for (size_t i = 0; i < element["nodes"].size(); ++i) {
        if (element["nodes"][i] == repeat_node) continue;

        double bearing = get_bearing(lat, lon, element["geometry"][i]["lat"],
                                     element["geometry"][i]["lon"]);
        double distance =
            calculate_distance(lat, lon, element["geometry"][i]["lat"],
                               element["geometry"][i]["lon"]);
        double weight = 1.0 / (distance + 1e-10);
        bearings.push_back(bearing);
        weights.push_back(weight);
      }

      if (!bearings.empty()) {
        double weighted_sum = 0.0;
        double total_weight = 0.0;

        for (size_t i = 0; i < bearings.size(); ++i) {
          weighted_sum += bearings[i] * weights[i];
          total_weight += weights[i];
        }
        double weighted_avg_bearing = weighted_sum / total_weight;
        std::cout << "weighted_avg_bearing:" << weighted_avg_bearing
                  << std::endl;
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "JSON parse error: " << e.what() << std::endl;
  }
  return 0;
}
