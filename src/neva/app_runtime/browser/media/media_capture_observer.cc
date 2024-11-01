// Copyright 2024 LG Electronics, Inc.
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

#include "neva/app_runtime/browser/media/media_capture_observer.h"

MediaCaptureObserver::MediaCaptureObserver() = default;

MediaCaptureObserver::~MediaCaptureObserver() = default;

void MediaCaptureObserver::OnIsCapturingAudioChanged(
    content::WebContents* web_contents,
    bool is_capturing_audio) {
  browser::MediaCaptureServiceImpl::Get()->NotifyAudioCaptureState(
      is_capturing_audio);
}

void MediaCaptureObserver::OnIsCapturingVideoChanged(
    content::WebContents* web_contents,
    bool is_capturing_video) {
  browser::MediaCaptureServiceImpl::Get()->NotifyVideoCaptureState(
      is_capturing_video);
}

void MediaCaptureObserver::OnIsCapturingWindowChanged(
    content::WebContents* web_contents,
    bool is_capturing_window) {
  browser::MediaCaptureServiceImpl::Get()->NotifyWindowCaptureState(
      is_capturing_window);
}

void MediaCaptureObserver::OnIsCapturingDisplayChanged(
    content::WebContents* web_contents,
    bool is_capturing_display) {
  browser::MediaCaptureServiceImpl::Get()->NotifyDisplayCaptureState(
      is_capturing_display);
}
 