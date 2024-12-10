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
  }
  http::write(socket, res);
}