/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef _LINECPP_LINE_TRANSPORT_CUSTOM_HTTP_CLIENT_HPP_
#define _LINECPP_LINE_TRANSPORT_CUSTOM_HTTP_CLIENT_HPP_

#include <thrift/transport/THttpTransport.h>

using namespace apache::thrift;
using namespace apache::thrift::transport;

/**
 * @brief Client transport using HTTP. The path is an optional field that is
 * not required by Thrift HTTP server or client. It can be used i.e. with HTTP
 * redirection, load balancing or forwarding on the server.
 */
class TLineHttpClient : public THttpTransport {
public:
  /**
   * @brief Constructor that wraps an existing transport, but also sets the
   * host and path. The host and path are not used for the connection but are
   * set in the HTTP header of the transport.
   */
  TLineHttpClient(std::shared_ptr<TTransport> transport,
              std::string host = "localhost",
              std::string path = "/service",
              std::shared_ptr<TConfiguration> config = nullptr);

  /**
   * @brief Constructor that will create a new socket transport using the host
   * and port.
   */
  TLineHttpClient(std::string host, int port, 
              std::string path = "",
              std::shared_ptr<TConfiguration> config = nullptr);

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
