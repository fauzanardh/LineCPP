#ifndef _LINECPP_LINE_TRANSPORT_CUSTOM_HTTP_CLIENT_HPP_
#define _LINECPP_LINE_TRANSPORT_CUSTOM_HTTP_CLIENT_HPP_

#include <map>
#include <thrift/transport/THttpTransport.h>

using namespace apache::thrift::transport;

/**
 * @brief Client transport using HTTP. The path is an optional field that is
 * not required by Thrift HTTP server or client. It can be used i.e. with HTTP
 * redirection, load balancing or forwarding on the server.
 */
class TLineHttpClient : public THttpTransport {
public:
  /**
   * @brief Constructor that will create a new socket transport using the host
   * and port.
   */
  TLineHttpClient(std::string host, int port, std::string path = "");

  ~TLineHttpClient() override;

  void flush() override;

  void setPath(std::string path);
  void addHeader(std::string key, std::string value);

protected:
  std::string host_;
  std::string path_;
  std::map<std::string, std::string> *headers_;

  void parseHeader(char* header) override;
  bool parseStatusLine(char* status) override;
};


#endif /* _LINECPP_LINE_TRANSPORT_CUSTOM_HTTP_CLIENT_HPP_ */
