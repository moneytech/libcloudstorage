/*****************************************************************************
 * GetItemDataRequest.cpp : GetItemDataRequest implementation
 *
 *****************************************************************************
 * Copyright (C) 2016-2016 VideoLAN
 *
 * Authors: Paweł Wegner <pawel.wegner95@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include "GetItemDataRequest.h"

#include <iostream>

#include "CloudProvider/CloudProvider.h"
#include "Utility/Utility.h"

namespace cloudstorage {

GetItemDataRequest::GetItemDataRequest(std::shared_ptr<CloudProvider> p,
                                       const std::string& id, Callback callback,
                                       Factory f)
    : Request(p), id_(id), callback_(callback) {
  if (f)
    function_ = std::async(std::launch::async, [this, f]() {
      auto item = f(this);
      callback_(item);
      return item;
    });
  else
    function_ = std::async(std::launch::async,
                           std::bind(&GetItemDataRequest::resolve, this));
}

GetItemDataRequest::~GetItemDataRequest() { cancel(); }

GetItemDataRequest::Callback GetItemDataRequest::callback() const {
  return callback_;
}

void GetItemDataRequest::finish() {
  if (function_.valid()) function_.wait();
}

IItem::Pointer GetItemDataRequest::result() {
  std::shared_future<IItem::Pointer> future = function_;
  if (!future.valid()) throw std::logic_error("Future invalid.");
  return future.get();
}

void GetItemDataRequest::error(int code, const std::string& error) {
  std::cerr << "[FAIL] Code " << code << ": " << error << "\n";
}

IItem::Pointer GetItemDataRequest::resolve(GetItemDataRequest* t) {
  std::stringstream response_stream;
  int code = t->sendRequest(
      [t](std::ostream& input) {
        return t->provider()->getItemDataRequest(t->id_, input);
      },
      response_stream);
  if (HttpRequest::isSuccess(code)) {
    auto i = t->provider()->getItemDataResponse(response_stream);
    t->callback_(i);
    return i;
  }
  t->callback_(nullptr);
  return nullptr;
}

}  // namespace cloudstorage
