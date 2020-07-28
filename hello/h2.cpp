#include <cpprest/filestream.h>
#include <cpprest/http_client.h>

using namespace utility;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

int main(int argc, char* args[])
{
    const string_t outputFileName = "out";

    // Open a stream to the file to write the HTTP response body into.
    auto fileBuffer = std::make_shared<streambuf<uint8_t>>();

    file_buffer<uint8_t>::open(outputFileName, std::ios::out)
        .then([=](streambuf<uint8_t> outFile) -> pplx::task<http_response> {
            *fileBuffer = outFile;

            // Create an HTTP request.
            // Encode the URI query since it could contain special characters like spaces.
            http_client client(U("https://t-azmaps.azurelbs.com/"));
            return client.request(methods::GET, uri_builder(U("turnbyturn/probesconnectionstring")).
            append_query(U("api-version"), "1.0").
            append_query(U("subscription-key"), "pZtLHCEeR67jVc6biKoo_xQlZZEQltgVTDRKHnbTjvs").
            append_query(U("DeviceId"), "sean456").
            to_string());
        })

        // Write the response body into the file buffer.
        .then([=](http_response response) -> pplx::task<size_t> {
            printf("Response status code %u returned.\n", response.status_code());

            return response.body().read_to_end(*fileBuffer);
        })

        // Close the file buffer.
        .then([=](size_t) { return fileBuffer->close(); })

        // Wait for the entire response body to be written into the file.
        .wait();

    return 0;
}