#include <cstdio>

#include <thrift/transport/THttpClient.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TBufferTransports.h>

#include <line/transport/custom_http_client.hpp>
#include <generated/SecondaryQrCodeService/SecondaryQrCodeLoginService.h>

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace LineThrift::SecondaryQrCode;

int main() {
    // auto http_client = shared_ptr<THttpClient>(new THttpClient("legy-jp.line.naver.jp", 443, "/acct/lgn/sq/v1"));
    auto http_client = shared_ptr<TLineHttpClient>(new TLineHttpClient("legy-jp.line.naver.jp", 443, "/acct/lgn/sq/v1"));
    // http_client->addHeader("User-Agent", "LLA/2.11.1 Redmi K20 Pro 10");
    // http_client->addHeader("X-Line-Application", "ANDROIDLITE\t2.11.1\tAndroid OS\t10;SECONDARY");
    auto transport = shared_ptr<TBufferedTransport>(new TBufferedTransport(http_client));
    auto protocol = shared_ptr<TCompactProtocol>(new TCompactProtocol(transport));
    SecondaryQrCodeLoginServiceClient client(protocol);
    
    try {
        http_client->open();
        CreateQrSessionResponse response;
        client.createSession(response, CreateQrSessionRequest());
        printf("AuthSessionId: %s\n", response.authSessionId);
        http_client->close();
    } catch (TException tx) {
        printf("Error: %s\n", tx.what());
    }
}