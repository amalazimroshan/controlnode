#include <controlnode/data_process.hpp>
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

std::string read_file(const std::string& file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + file_path);
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

std::unordered_map<std::string, std::string> parse_from_urlencoded(
    const std::string& body) {
  std::unordered_map<std::string, std::string> params;
  std::istringstream iss(body);
  std::string param;

  while (std::getline(iss, param, '&')) {
    size_t pos = param.find('=');
    if (pos != std::string::npos) {
      std::string key = param.substr(0, pos);
      std::string value = param.substr(pos + 1);
      params[key] = value;
    }
  }
  return params;
}
void handle_request(tcp::socket& socket) {
  boost::beast::flat_buffer buffer;
  http::request<http::string_body> req;
  http::response<http::string_body> res{http::status::ok, 11};
  http::read(socket, buffer, req);

  if (req.method() == http::verb::get) {
    std::string target = req.target();
    if (target == "/") {
      std::string html_content = read_file("../index.html");
      res.set(http::field::content_type, "text/html");
      res.body() = html_content;
      res.prepare_payload();
    } else if (target == "/ways") {
      std::string roadways_json = serialize_roadways_tojson(ways);
      res.set(http::field::content_type, "application/json");
      res.body() = roadways_json;
      res.prepare_payload();
    }
  } else if (req.method() == http::verb::post) {
    std::string body = req.body();

    try {
      auto params = parse_from_urlencoded(body);
      if (params.find("id") == params.end()) {
        throw std::runtime_error("No ID found");
      }

      int id = std::stoi(params["id"]);
      auto it = std::find_if(ways.begin(), ways.end(),
                             [id](const Roadway& way) { return way.id == id; });

      if (it != ways.end()) {
        if (params.find("bearing") != params.end()) {
          it->bearing = std::stod(params["bearing"]);
        }
        if (params.find("time") != params.end()) {
          it->time = std::stoi(params["time"]);
        }
        it->closed = (params.find("closed") != params.end());

        res.body() = "Roadway updated successfully";
      } else {
        res.result(http::status::not_found);
        res.body() = "Roadway not found";
      }
    } catch (std::exception& e) {
      res.result(http::status::bad_request);
      res.body() = std::string("Error: ") + e.what();
      std::cerr << "form parameter error " << e.what() << std::endl;
    }
    res.prepare_payload();
  }
  http::write(socket, res);
}