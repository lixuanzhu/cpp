#include <cpprest/http_client.h>

using namespace web::http;
using namespace web::http::client;

int main(int argc, char* args[])
{
    http_client_config config;
    config.set_validate_certificates(false);
    http_client client(U("https://192.168.50.43/"), config);
    std::string uri = uri_builder(U("turnbyturn/probesconnectionstring")).
        append_query(U("api-version"), "1.0").
        append_query(U("subscription-key"), "pZtLHCEeR67jVc6biKoo_xQlZZEQltgVTDRKHnbTjvs").
        append_query(U("DeviceId"), "sean456").
        append_query(U("AppId"), "app456").
        to_string();

    client.request(methods::GET, uri)
    .then([=](http_response response) {
        std::cout << response.status_code() << std::endl;
        std::cout << response.extract_json().get()["hubEndpoint"].as_string() << std::endl;
    })
    .wait();

    return 0;
}