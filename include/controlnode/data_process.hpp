#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct GeoPoint {
  double lat;
  double lon;
};
struct Roadway {
  std::vector<GeoPoint> points;
  double bearing;
};
double get_bearing(const GeoPoint& point1, const GeoPoint& point2);
std::vector<Roadway> extract_roadways(const std::string& data);
std::string serialize_roadways_tojson(const std::vector<Roadway>& ways);