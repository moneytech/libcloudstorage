/*****************************************************************************
 * HttpRequest.h : interface of HttpRequest
 *
 *****************************************************************************
 * Copyright (C) 2016-2016 VideoLAN
 *
 * Authors: Paweł Wegner <pawel.wegner95@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <curl/curl.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace cloudstorage {

class HttpRequest {
 public:
  enum class Type { POST, GET };

  HttpRequest(const std::string& url, Type);

  void setParameter(const std::string& parameter, const std::string& value);
  void setHeaderParameter(const std::string& parameter,
                          const std::string& value);

  const std::string& url() const;
  void set_url(const std::string&);

  const std::string& post_data() const;
  void set_post_data(const std::string&);

  Type type() const;
  void set_type(Type);

  std::string send() const;
  void reset_parameters();

 private:
  struct CurlDeleter {
    bool operator()(CURL*) const;
  };

  std::string parametersToString() const;
  curl_slist* headerParametersToList() const;

  std::unique_ptr<CURL, CurlDeleter> handle_;
  std::string url_;
  std::string post_data_;
  std::unordered_map<std::string, std::string> parameters_;
  std::unordered_map<std::string, std::string> header_parameters_;
  Type type_;
};

}  // namespace cloudstorage

#endif  // HTTPREQUEST_H