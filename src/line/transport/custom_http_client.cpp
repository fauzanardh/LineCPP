#include <iostream>
#include <limits>
#include <cstdlib>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include <thrift/config.h>
#include <thrift/transport/TSocket.h>
#include <line/transport/custom_http_client.hpp>
#include <utils/base64.hpp>

using std::string;
using namespace apache::thrift::transport;

TLineHttpClient::TLineHttpClient(string host, int port, string path)
  : THttpTransport(std::shared_ptr<TTransport>(new TSocket(host, port))),
    host_(host),
    path_(path),
    headers_(new std::map<std::string, std::string>{}) {
}

TLineHttpClient::~TLineHttpClient() = default;

void TLineHttpClient::parseHeader(char* header) {
  char* colon = strchr(header, ':');
  if (colon == nullptr) {
    return;
  }
  char* value = colon + 1;

  if (boost::istarts_with(header, "Transfer-Encoding")) {
    if (boost::iends_with(value, "chunked")) {
      chunked_ = true;
    }
  } else if (boost::istarts_with(header, "Content-Length")) {
    chunked_ = false;
    contentLength_ = atoi(value);
  }
}

bool TLineHttpClient::parseStatusLine(char* status) {
  char* http = status;

  char* code = strchr(http, ' ');
  if (code == nullptr) {
    throw TTransportException(string("Bad Status: ") + status);
  }

  *code = '\0';
  while (*(code++) == ' ') {
  };

  char* msg = strchr(code, ' ');
  if (msg == nullptr) {
    throw TTransportException(string("Bad Status: ") + status);
  }
  *msg = '\0';

  if (strcmp(code, "200") == 0) {
    // HTTP 200 = OK, we got the response
    return true;
  } else if (strcmp(code, "100") == 0) {
    // HTTP 100 = continue, just keep reading
    return false;
  } else {
    throw TTransportException(string("Bad Status: ") + status);
  }
}

void TLineHttpClient::flush() {
  resetConsumedMessageSize();
  // Fetch the contents of the write buffer
  uint8_t* buf;
  uint32_t len;
  writeBuffer_.getBuffer(&buf, &len);

  // Construct the HTTP header
  std::ostringstream h;
  h << "POST " << path_ << " HTTP/1.1" << CRLF
    << "Host: " << host_ << CRLF
    << "Accept: application/x-thrift" << CRLF
    << "Connection: keep-alive" << CRLF
    << "Content-Type: application/x-thrift" << CRLF
    << "Content-Length: " << len << CRLF;

  for (auto const& m : *headers_) {
    h << m.first << ": " << m.second << CRLF;
  }
  h << CRLF << CRLF;
  string header = h.str();

  std::cout << "Headers:\n" << header << "Body:\n" << LineCPP::Base64().Encode(std::string(reinterpret_cast<char *>(buf))) << "\n";

  if (header.size() > (std::numeric_limits<uint32_t>::max)())
    throw TTransportException("Header too big");
  // Write the header, then the data, then flush
  transport_->write((const uint8_t*)header.c_str(), static_cast<uint32_t>(header.size()));
  transport_->write(buf, len);
  transport_->flush();

  // Reset the buffer and header variables
  writeBuffer_.resetBuffer();
  readHeaders_ = true;
}

void TLineHttpClient::setPath(std::string path) {
  path_ = path;
}

void TLineHttpClient::addHeader(std::string key, std::string value) 
{
  headers_->insert({key, value});
}