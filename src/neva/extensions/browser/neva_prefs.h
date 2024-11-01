// Copyright 2022 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef NEVA_EXTENSIONS_BROWSER_NEVA_PREFS_H_
#define NEVA_EXTENSIONS_BROWSER_NEVA_PREFS_H_

#include <memory>

class PrefService;

namespace base {
class FilePath;
}

namespace content {
class BrowserContext;
}

namespace neva {

// Support for preference initialization and management.
namespace prefs {

// Creates a pref service for device-wide preferences stored in |data_dir|.
std::unique_ptr<PrefService> CreateLocalState(const base::FilePath& data_dir);

}  // namespace prefs

}  // namespace neva

#endif  // NEVA_EXTENSIONS_BROWSER_NEVA_PREFS_H_
