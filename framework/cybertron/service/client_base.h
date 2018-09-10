/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef CYBERTRON_SERVICE_CLIENT_BASE_H_
#define CYBERTRON_SERVICE_CLIENT_BASE_H_

#include <chrono>
#include <string>

#include "cybertron/common/macros.h"

namespace apollo {
namespace cybertron {

class ClientBase {
 public:
  explicit ClientBase(const std::string& service_name)
      : service_name_(service_name) {}
  virtual ~ClientBase() {}

  virtual void Destroy() = 0;

  const std::string& ServiceName() const { return service_name_; }

  bool ServiceIsReady() const;

 protected:
  std::string service_name_;

  bool WaitForServiceNanoseconds(std::chrono::nanoseconds time_out) {
    bool has_service = false;
    auto step_duration = std::chrono::nanoseconds(5 * 1000 * 1000);
    while (!has_service && time_out.count() > 0) {
      has_service =
          topology::Topology::Instance()->service_manager()->HasService(
              service_name_);
      std::this_thread::sleep_for(step_duration);
      time_out -= step_duration;
    }
    return has_service;
  }
};

}  // namespace cybertron
}  // namespace apollo

#endif  // CYBERTRON_SERVICE_CLIENT_BASE_H_
