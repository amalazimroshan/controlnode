
#include <controlnode/data_process.hpp>
#include <controlnode/interface.hpp>
#include <iostream>

int main() {
  double lat = 8.523488;
  double lon = 76.945289;
  const int port = 8080;
  asio::io_context ioc;
  tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), port));
  // double lat = 8.519897;
  // double lon = 76.945056;
  std::string overpass_url =
      "https://overpass-api.de/api/"
      "interpreter?data=[out:json];(node(around:10," +
      std::to_string(lat) + "," + std::to_string(lon) +
      ")->.mainnode;);.mainnode%20out%20geom;way(bn)[\"highway\"~\"^("
      "motorway|"
      "trunk|primary|secondary|"
      "tertiary|unclassified|residential|service|living_street)$\"];"
      "out%20geom;";
  std::string response = fetch_overpass_data(overpass_url);
  extract_roadways(response);
  // std::string roadways_json = serialize_roadways_tojson(ways);
  // std::cout << roadways_json << std::endl;
  while (true) {
    tcp::socket socket(ioc);
    acceptor.accept(socket);
    try {
      handle_request(socket);
    } catch (const std::exception& e) {
      std::cerr << "Request handling error " << e.what() << std::endl;
    }
  }

  return 0;
}
