// Copyright 2017-2018 LG Electronics, Inc.
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

#include "ozone/ui/webui/input_method_context_manager.h"

#include "base/functional/bind.h"
#include "base/logging.h"
#include "base/task/single_thread_task_runner.h"
#include "ozone/platform/messages.h"
#include "ozone/platform/ozone_gpu_platform_support_host.h"
#include "ozone/ui/webui/input_method_context_impl_wayland.h"

namespace ui {

InputMethodContextManager::InputMethodContextManager(OzoneGpuPlatformSupportHost* sender)
    : sender_(sender),
      weak_ptr_factory_(this) {
  sender_->RegisterHandler(this);
}

InputMethodContextManager::~InputMethodContextManager() {
  sender_->UnregisterHandler(this);
}

void InputMethodContextManager::AddContext(InputMethodContextImplWayland* context) {
  contexts_list_.push_back(context);
}

void InputMethodContextManager::RemoveContext(InputMethodContextImplWayland* context) {
  contexts_list_.remove(context);
}

void InputMethodContextManager::ImeReset(unsigned handle) {
  sender_->Send(new WaylandDisplay_ImeReset(handle));
}

void InputMethodContextManager::OnGpuProcessLaunched(
    int host_id,
    base::RepeatingCallback<void(IPC::Message*)> sender) {}

void InputMethodContextManager::OnGpuServiceLaunched(
    int host_id,
    GpuHostBindInterfaceCallback binder,
    GpuHostTerminateCallback terminate_callback) {}

void InputMethodContextManager::OnChannelDestroyed(int host_id) {
}

void InputMethodContextManager::OnMessageReceived(const IPC::Message& message) {
  IPC_BEGIN_MESSAGE_MAP(InputMethodContextManager, message)
  IPC_MESSAGE_HANDLER(WaylandInput_Commit, Commit)
  IPC_MESSAGE_HANDLER(WaylandInput_DeleteRange, DeleteRange)
  IPC_MESSAGE_HANDLER(WaylandInput_PreeditChanged, PreeditChanged)
  IPC_MESSAGE_HANDLER(WaylandInput_PreeditEnd, PreeditEnd)
  IPC_MESSAGE_HANDLER(WaylandInput_PreeditStart, PreeditStart)
  IPC_END_MESSAGE_MAP()
}

void InputMethodContextManager::NotifyCommit(unsigned handle, const std::string& text) {
  auto* context = GetContext(handle);
  if (!context) {
    LOG(INFO) << "Context is not found for current handle window id";
    return;
  }
  context->Commit(text);
}

void InputMethodContextManager::NotifyPreeditChanged(unsigned handle,
                                                     const std::string& text,
                                                     const std::string& commit) {
  auto* context = GetContext(handle);
  if (!context) {
    LOG(INFO) << "Context is not found for current handle window id";
    return;
  }
  context->PreeditChanged(text, commit);
}

void InputMethodContextManager::NotifyPreeditEnd() {
}

void InputMethodContextManager::NotifyPreeditStart() {
}

void InputMethodContextManager::NotifyDeleteRange(unsigned handle,
                                                  int32_t index,
                                                  uint32_t length) {
  auto* context = GetContext(handle);
  if (!context) {
    LOG(INFO) << "Context is not found for current handle window id";
    return;
  }
  context->DeleteRange(index, length);
}

void InputMethodContextManager::Commit(unsigned handle, const std::string& text) {
  base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE, base::BindOnce(&InputMethodContextManager::NotifyCommit,
                                weak_ptr_factory_.GetWeakPtr(), handle, text));
}

void InputMethodContextManager::PreeditChanged(unsigned handle,
                                               const std::string& text,
                                               const std::string& commit) {
  base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE,
      base::BindOnce(&InputMethodContextManager::NotifyPreeditChanged,
                     weak_ptr_factory_.GetWeakPtr(), handle, text, commit));
}

void InputMethodContextManager::DeleteRange(unsigned handle,
                                            int32_t index,
                                            uint32_t length) {
  base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE,
      base::BindOnce(&InputMethodContextManager::NotifyDeleteRange,
                     weak_ptr_factory_.GetWeakPtr(), handle, index, length));
}

void InputMethodContextManager::PreeditEnd() {
  base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE, base::BindOnce(&InputMethodContextManager::NotifyPreeditEnd,
                                weak_ptr_factory_.GetWeakPtr()));
}

void InputMethodContextManager::PreeditStart() {
  base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE, base::BindOnce(&InputMethodContextManager::NotifyPreeditStart,
                                weak_ptr_factory_.GetWeakPtr()));
}

InputMethodContextImplWayland* InputMethodContextManager::GetContext(unsigned handle) {
  InputMethodContextImplWayland* context = nullptr;

  for (auto it = contexts_list_.begin(); it != contexts_list_.end(); ++it) {
    if ((*it)->GetHandle() == handle) {
      context = *it;
      break;
    }
  }

  return context;
}

}  // namespace ui
