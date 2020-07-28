#include <cpr/cpr.h>
#include <iostream>

int main(int argc, char** argv) {
    auto r = cpr::Get(cpr::Url{"https://t-azmaps.azurelbs.com/turnbyturn/probesconnectionstring"},
                      cpr::Parameters{{"api-version", "1.0"}, 
                                      {"subscription-key", "pZtLHCEeR67jVc6biKoo_xQlZZEQltgVTDRKHnbTjvs"},
                                      {"DeviceId", "sean456"}});
    std::cout << r.status_code << std::endl;                  // 200
    std::cout << r.header["content-type"]  << std::endl;       // application/json; charset=utf-8
    std::cout << r.text  << std::endl;                         // JSON text string
}
