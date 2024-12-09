
#include <controlnode/data_process.hpp>
#include <controlnode/interface.hpp>
#include <iostream>

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
  double lat = 8.523488;
  double lon = 76.945289;
  // double lat = 8.519897;
  // double lon = 76.945056;
  std::string overpassUrl =
      "https://overpass-api.de/api/"
      "interpreter?data=[out:json];(node(around:10," +
      std::to_string(lat) + "," + std::to_string(lon) +
      ")->.mainnode;);.mainnode%20out%20geom;way(bn)[\"highway\"~\"^(motorway|"
      "trunk|primary|secondary|"
      "tertiary|unclassified|residential|service|living_street)$\"];"
      "out%20geom;";
  std::string response = fetch_overpass_data(overpassUrl);
  std::vector<Roadway> ways = extract_roadways(response);

  return 0;
}
