// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// RMT DECUP encoder
///
/// \file   rmt_decup_encoder.h
/// \author Vincent Hamp
/// \date   14/08/2024

#pragma once

#include <driver/rmt_encoder.h>

#ifdef __cplusplus
extern "C" {
#endif

/// DECUP encoder configuration
typedef struct {
  struct {
    /// Invert RMT symbol level
    ///
    /// This boolean value corresponds to the level of the startbit.
    bool invert : 1;
  } flags;
} decup_encoder_config_t;

/// Create RMT DECUP encoder which encodes DECUP byte stream into RMT symbols
///
/// \param  config              DECUP encoder configuration
/// \param  ret_encoder         Returned encoder handle
/// \retval ESP_OK              Create RMT DECUP encoder successfully
/// \retval ESP_ERR_INVALID_ARG Create RMT DECUP encoder failed because of
///                             invalid argument
/// \retval ESP_ERR_NO_MEM      Create RMT DECUP encoder failed because out of
///                             memory
/// \retval ESP_FAIL            Create RMT DECUP encoder failed because of other
///                             error
esp_err_t rmt_new_decup_encoder(decup_encoder_config_t const* config,
                                rmt_encoder_handle_t* ret_encoder);

#ifdef __cplusplus
}
#endif