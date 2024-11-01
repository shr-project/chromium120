// Copyright 2015 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import {dispatchSimpleEvent} from 'chrome://resources/ash/common/cr_deprecated.js';
import {NativeEventTarget as EventTarget} from 'chrome://resources/ash/common/event_target.js';

export class MockActionModel extends EventTarget {
  /**
   * @param {string} title
   * @param {Array<!Entry>} entries
   */
  constructor(title, entries) {
    super();

    this.title = title;
    this.entries = entries;
    this.actionsModel = null;
  }

  getTitle() {
    return this.title;
  }

  onCanExecute() {}

  onExecute() {
    dispatchSimpleEvent(this, 'invalidated', true);
  }
}

export class MockActionsModel extends EventTarget {
  // @ts-ignore: error TS7006: Parameter 'actions' implicitly has an 'any' type.
  constructor(actions) {
    super();

    this.actions_ = actions;
    Object.keys(actions).forEach(function(key) {
      // @ts-ignore: error TS2683: 'this' implicitly has type 'any' because it
      // does not have a type annotation.
      actions[key].actionsModel = this;
    });
  }

  initialize() {
    return Promise.resolve();
  }

  getActions() {
    return this.actions_;
  }
}
