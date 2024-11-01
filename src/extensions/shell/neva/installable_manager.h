// Copyright 2023 LG Electronics, Inc.
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

#ifndef EXTENSIONS_SHELL_NEVA_INSTALLABLE_MANAGER_H_
#define EXTENSIONS_SHELL_NEVA_INSTALLABLE_MANAGER_H_

#include "content/public/browser/web_contents_user_data.h"
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "neva/app_runtime/browser/installable/webapp_installable_manager.h"
#include "neva/app_runtime/public/mojom/installable_manager.mojom.h"

namespace neva_app_runtime {

class InstallableManager
    : public content::WebContentsUserData<InstallableManager>,
      public mojom::InstallableManager {
 public:
  InstallableManager(content::WebContents* web_contents);
  ~InstallableManager() override;

  static void BindInstallableManager(
      mojo::PendingReceiver<mojom::InstallableManager> receiver,
      content::RenderFrameHost* rfh);

  // mojom::InstallableManager
  void GetInfo(GetInfoCallback callback) override;
  void InstallApp(InstallAppCallback callback) override;

 private:
  void OnGetInfo(GetInfoCallback callback, bool installable, bool installed);
  void OnInstallApp(InstallAppCallback callback, bool success);

  content::WebContents* web_contents_;
  mojo::ReceiverSet<mojom::InstallableManager> receivers_;
  WebAppInstallableManager installable_manager_;
  base::WeakPtrFactory<InstallableManager> weak_factory_;

  friend class content::WebContentsUserData<InstallableManager>;
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};

}  // namespace neva_app_runtime

#endif  // EXTENSIONS_SHELL_NEVA_INSTALLABLE_MANAGER_H_
