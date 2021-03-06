/*****************************************************************************
 * GetItemRequest.h : GetItemRequest headers
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

#ifndef GETITEMREQUEST_H
#define GETITEMREQUEST_H

#include "ListDirectoryRequest.h"

namespace cloudstorage {

class GetItemRequest : public Request<EitherError<IItem>> {
 public:
  using Callback = GetItemCallback;

  GetItemRequest(std::shared_ptr<CloudProvider>, const std::string& path,
                 const Callback& callback);
  ~GetItemRequest() override;

 private:
  IItem::Pointer getItem(const IItem::List& items,
                         const std::string& name) const;
  void work(const IItem::Pointer& item, const std::string& path,
            const Callback&);
};

}  // namespace cloudstorage

#endif  // GETITEMREQUEST_H
