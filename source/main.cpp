
#include <controlnode/data_process.hpp>
#include <controlnode/interface.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
  double lat = 8.523488;
  double lon = 76.945289;
  if (argc >= 3) {  // expect 2 arguments after the program name
    lat = std::atof(argv[1]);
    lon = std::atof(argv[2]);
  }
  std::cout << "Latitude: " << lat << std::endl;
  std::cout << "Longitude: " << lon << std::endl;

  const int port = 8080;
  asio::io_context ioc;
  tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), port));

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
    std::cout << "server listening on http://localhost:8080/" << std::endl;
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
