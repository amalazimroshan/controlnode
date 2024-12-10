#include <controlnode/data_process.hpp>

std::vector<Roadway> ways;

double get_bearing(const GeoPoint& point1, const GeoPoint& point2) {
  double lat1 = point1.lat * M_PI / 180;
  double lon1 = point1.lon * M_PI / 180;
  double lat2 = point2.lat * M_PI / 180;
  double lon2 = point2.lon * M_PI / 180;
  double dLon = lon2 - lon1;

  double y = sin(dLon) * cos(lat2);
  double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);

  double bearing_rad = atan2(y, x);
  double bearing_deg = fmod(bearing_rad * 180.0 / M_PI + 360, 360.0);
  return bearing_deg;
}
void extract_roadways(const std::string& raw_response) {
  json data = json::parse(raw_response);
  uint64_t controlnode_id = data["elements"][0]["id"];
  for (const auto& element : data["elements"]) {
    if (element["type"] == "way") {
      Roadway roadway;

      //   convert nodes to GeoPoints
      for (size_t i = 0; i < element["nodes"].size(); ++i) {
        roadway.points.push_back({
            element["geometry"][i]["lat"],  // lat
            element["geometry"][i]["lon"]   // lon
        });
      }

      // finding ways from controlnode (intersection node)
      // way starts with contorlnode
      if (element["nodes"][0] == controlnode_id) {
        roadway.bearing = get_bearing(roadway.points[0], roadway.points[1]);
        ways.push_back(roadway);
        // std::cout << "this way starts with control node" << std::endl;
      }

      // ways ends with controlnode
      else if (element["nodes"].back() == controlnode_id) {
        std::reverse(roadway.points.begin(), roadway.points.end());
        roadway.bearing = get_bearing(roadway.points[0], roadway.points[1]);
        ways.push_back(roadway);
        // std::cout << "this way ends with control node" << std::endl;
      }

      // control node is in the middle of the way
      else {
        auto it = std::find(element["nodes"].begin(), element["nodes"].end(),
                            controlnode_id);
        if (it != element["nodes"].end()) {
          size_t index = std::distance(element["nodes"].begin(), it);

          // forward segment
          Roadway forward_way;
          forward_way.points = std::vector<GeoPoint>(
              roadway.points.begin() + index,
              roadway.points.begin() +
                  std::min(index + 5, roadway.points.size()));

          forward_way.bearing =
              get_bearing(forward_way.points[0], forward_way.points[1]);
          // std::cout << "====forward way=====" << std::endl;
          // for (auto p : forward_way.points) {
          //   std::cout << p.lat << " " << p.lon << std::endl;
          // }
          ways.push_back(forward_way);

          // backward segment
          Roadway backward_way;
          backward_way.points = std::vector<GeoPoint>(
              roadway.points.begin() + std::max(0, static_cast<int>(index) - 4),
              roadway.points.begin() + index + 1);
          std::reverse(backward_way.points.begin(), backward_way.points.end());
          // std::cout << "====backward way=====" << std::endl;
          // for (auto p : backward_way.points) {
          //   std::cout << p.lat << " " << p.lon << std::endl;
          // }
          backward_way.bearing =
              get_bearing(backward_way.points[0], backward_way.points[1]);
          ways.push_back(backward_way);
        }
        // std::cout << "this way  have control node in middle" << std::endl;
      }
    }
  }
  // return ways;
}

std::string serialize_roadways_tojson(const std::vector<Roadway>& ways) {
  json ways_json = json::array();

  for (const auto& way : ways) {
    json way_json{{"bearing", way.bearing},
                  {"points", json::array()},
                  {"time", "30"},
                  {"closed", json::boolean_t(0)}};

    for (const auto& point : way.points) {
      way_json["points"].push_back({
          {"lat", point.lat},
          {"lon", point.lon},
      });
    }
    ways_json.push_back(way_json);
  }
  return ways_json.dump();
}