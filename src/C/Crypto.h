/*****************************************************************************
 * Crypto.h
 *
 *****************************************************************************
 * Copyright (C) 2016-2018 VideoLAN
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
#ifndef CRYPTO_H
#define CRYPTO_H

#include "Item.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

struct cloud_crypto;
struct cloud_array {
  const void* data;
  size_t length;
};

struct cloud_crypto_operations {
  struct cloud_array (*sha256)(struct cloud_array message, void* userdata);
  struct cloud_array (*hmac_sha256)(struct cloud_array key,
                                    struct cloud_array message, void* userdata);
  struct cloud_array (*hmac_sha1)(struct cloud_array key,
                                  struct cloud_array message, void* userdata);
  struct cloud_array (*hex)(struct cloud_array hash, void* userdata);
  void (*release)(void* userdata);
};

CLOUDSTORAGE_API struct cloud_crypto* cloud_crypto_create_default();
CLOUDSTORAGE_API struct cloud_crypto* cloud_crypto_create(
    struct cloud_crypto_operations*, void*);
CLOUDSTORAGE_API void cloud_crypto_release(struct cloud_crypto*);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // CRYPTO_H
