// Copyright 2014 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_SHELL_BROWSER_SHELL_BROWSER_MAIN_PARTS_H_
#define EXTENSIONS_SHELL_BROWSER_SHELL_BROWSER_MAIN_PARTS_H_

#include <memory>

#include "base/compiler_specific.h"
#include "base/memory/raw_ptr.h"
#include "build/build_config.h"
#include "build/chromeos_buildflags.h"
#include "components/nacl/common/buildflags.h"
#include "content/public/browser/browser_main_parts.h"

#if defined(USE_NEVA_BROWSER_SERVICE)
#include "extensions/shell/neva/shell_media_capture_observer.h"
#include "extensions/shell/neva/shell_permission_prompt.h"
#endif

class PrefService;

#if defined(USE_NEVA_APPRUNTIME)
namespace neva_app_runtime {
class AppRuntimeSharedMemoryManager;
}
#endif

#if defined(USE_NEVA_BROWSER_SERVICE)
namespace neva {
class MalwareDetectionService;
class ShellMediaCaptureObserver;
}  // namespace neva
#endif

namespace extensions {

class DesktopController;
class ShellBrowserContext;
class ShellBrowserMainDelegate;
class ShellExtensionsClient;
class ShellExtensionsBrowserClient;
class ShellExtensionSystem;
class ShellUpdateQueryParamsDelegate;

#if BUILDFLAG(IS_CHROMEOS_ASH)
class ShellAudioController;
class ShellNetworkController;
#endif

// Handles initialization of AppShell.
class ShellBrowserMainParts : public content::BrowserMainParts {
 public:
  ShellBrowserMainParts(ShellBrowserMainDelegate* browser_main_delegate,
                        bool is_integration_test);

  ShellBrowserMainParts(const ShellBrowserMainParts&) = delete;
  ShellBrowserMainParts& operator=(const ShellBrowserMainParts&) = delete;

  ~ShellBrowserMainParts() override;

  ShellBrowserContext* browser_context() { return browser_context_.get(); }

  ShellExtensionSystem* extension_system() { return extension_system_; }

  // BrowserMainParts overrides.
  int PreEarlyInitialization() override;
  ///@name USE_NEVA_APPRUNTIME
  ///@{
  void ToolkitInitialized() override;
  ///@}
  void PostCreateMainMessageLoop() override;
  int PreCreateThreads() override;
  int PreMainMessageLoopRun() override;
  void WillRunMainMessageLoop(
      std::unique_ptr<base::RunLoop>& run_loop) override;
  void PostMainMessageLoopRun() override;
  void PostDestroyThreads() override;

#if defined(USE_NEVA_BROWSER_SERVICE)
  // Store instance of malware detection service
  neva::MalwareDetectionService* malware_detection_service() {
    return malware_detection_service_.get();
  }
#endif

 private:
  // Initializes the ExtensionSystem.
  void InitExtensionSystem();

#if defined(OS_WEBOS)
  void ExitWhenPossibleOnUIThread(int signal);
#endif

#if BUILDFLAG(IS_CHROMEOS_ASH)
  std::unique_ptr<ShellNetworkController> network_controller_;
#endif

  std::unique_ptr<ShellBrowserContext> browser_context_;
  std::unique_ptr<PrefService> local_state_;
  std::unique_ptr<PrefService> user_pref_service_;

#if BUILDFLAG(IS_CHROMEOS_ASH)
  std::unique_ptr<ShellAudioController> audio_controller_;
#endif

#if defined(USE_NEVA_APPRUNTIME)
  std::unique_ptr<neva_app_runtime::AppRuntimeSharedMemoryManager>
      app_runtime_mem_manager_;
#endif

#if defined(USE_NEVA_BROWSER_SERVICE)
  // Store instance of malware detection service
  scoped_refptr<neva::MalwareDetectionService> malware_detection_service_;

  std::unique_ptr<NevaPermissionsClientDelegate>
      neva_permission_client_delegate_;

  std::unique_ptr<ShellMediaCaptureObserver> shell_media_capture_observer_;
#endif

  // The DesktopController outlives ExtensionSystem and context-keyed services.
  std::unique_ptr<DesktopController> desktop_controller_;

  std::unique_ptr<ShellExtensionsClient> extensions_client_;
  std::unique_ptr<ShellExtensionsBrowserClient> extensions_browser_client_;
  std::unique_ptr<ShellUpdateQueryParamsDelegate> update_query_params_delegate_;

  // Owned by the KeyedService system.
  raw_ptr<ShellExtensionSystem, DanglingUntriaged> extension_system_;

  std::unique_ptr<ShellBrowserMainDelegate> browser_main_delegate_;

  const bool is_integration_test_;
};

}  // namespace extensions

#endif  // EXTENSIONS_SHELL_BROWSER_SHELL_BROWSER_MAIN_PARTS_H_
