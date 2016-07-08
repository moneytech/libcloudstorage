/*****************************************************************************
 * CloudProvider.cpp : implementation of CloudProvider
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

#include "CloudProvider.h"

#include <jsoncpp/json/json.h>
#include <sstream>

#include "Utility/Item.h"
#include "Utility/Utility.h"

namespace cloudstorage {

CloudProvider::CloudProvider(IAuth::Pointer auth)
    : auth_(std::move(auth)),
      currently_authorizing_(),
      current_authorization_successful_() {}

void CloudProvider::initialize(const std::string& token,
                               ICallback::Pointer callback,
                               const Hints& hints) {
  std::lock_guard<std::mutex> lock(auth_mutex_);
  callback_ = std::move(callback);
  auto t = auth()->fromTokenString(token);
  if (!hints.access_token_.empty()) t->token_ = hints.access_token_;
  auth()->set_access_token(std::move(t));
  if (!hints.client_id_.empty()) auth()->set_client_id(hints.client_id_);
  if (!hints.client_secret_.empty())
    auth()->set_client_secret(hints.client_secret_);
}

std::string CloudProvider::access_token() const {
  std::lock_guard<std::mutex> lock(auth_mutex_);
  if (auth()->access_token() == nullptr) return "";
  return auth()->access_token()->token_;
}

IAuth* CloudProvider::auth() const { return auth_.get(); }

std::string CloudProvider::authorizeLibraryUrl() const {
  return auth()->authorizeLibraryUrl();
}

std::string CloudProvider::token() const {
  std::lock_guard<std::mutex> lock(auth_mutex_);
  if (auth()->access_token() == nullptr) return "";
  return auth()->access_token()->refresh_token_;
}

IItem::Pointer CloudProvider::rootDirectory() const {
  return make_unique<Item>("/", "root", IItem::FileType::Directory);
}

ICloudProvider::ICallback* CloudProvider::callback() const {
  return callback_.get();
}

ListDirectoryRequest::Pointer CloudProvider::listDirectoryAsync(
    IItem::Pointer item, ListDirectoryRequest::ICallback::Pointer callback) {
  return make_unique<ListDirectoryRequest>(shared_from_this(), std::move(item),
                                           std::move(callback));
}

GetItemRequest::Pointer CloudProvider::getItemAsync(
    const std::string& absolute_path,
    std::function<void(IItem::Pointer)> callback) {
  return make_unique<GetItemRequest>(shared_from_this(), absolute_path,
                                     callback);
}

DownloadFileRequest::Pointer CloudProvider::downloadFileAsync(
    IItem::Pointer file, DownloadFileRequest::ICallback::Pointer callback) {
  return make_unique<DownloadFileRequest>(
      shared_from_this(), std::move(file), std::move(callback),
      std::bind(&CloudProvider::downloadFileRequest, this,
                std::placeholders::_1, std::placeholders::_2));
}

UploadFileRequest::Pointer CloudProvider::uploadFileAsync(
    IItem::Pointer directory, const std::string& filename,
    UploadFileRequest::ICallback::Pointer callback) {
  return make_unique<UploadFileRequest>(
      shared_from_this(), std::move(directory), filename, std::move(callback));
}

GetItemDataRequest::Pointer CloudProvider::getItemDataAsync(
    IItem::Pointer item, std::function<void(IItem::Pointer)> f) {
  return make_unique<GetItemDataRequest>(shared_from_this(), item, f);
}

void CloudProvider::authorizeRequest(HttpRequest& r) {
  r.setHeaderParameter("Authorization", "Bearer " + access_token());
}

AuthorizeRequest::Pointer CloudProvider::authorizeAsync() {
  return make_unique<AuthorizeRequest>(shared_from_this());
}

std::mutex& CloudProvider::auth_mutex() const { return auth_mutex_; }

DownloadFileRequest::Pointer CloudProvider::getThumbnailAsync(
    IItem::Pointer item, DownloadFileRequest::ICallback::Pointer callback) {
  return make_unique<DownloadFileRequest>(
      shared_from_this(), item, std::move(callback),
      std::bind(&CloudProvider::getThumbnailRequest, this,
                std::placeholders::_1, std::placeholders::_2));
}

}  // namespace cloudstorage