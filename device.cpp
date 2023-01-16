/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "device.h"

static const char* MENU_ITEMS[] = {
  "Reboot",
#if 0
  "Apply update from ADB",
  "Apply update from SD card",
#endif
  "Wipe data/factory reset",
#ifndef AB_OTA_UPDATER
  "Wipe cache partition",
#endif  // !AB_OTA_UPDATER
  "Wiper",
  "Mount /system",
  "View recovery logs",
#ifdef RECOVERY_TESTS
  "Run graphics test",
  "Run locale test",
#endif
  nullptr,
};

static const Device::BuiltinAction MENU_ACTIONS[] = {
  Device::REBOOTSEL,
#if 0
  Device::APPLY_ADB_SIDELOAD,
  Device::APPLY_SDCARD,
#endif
  Device::WIPE_DATA,
#ifndef AB_OTA_UPDATER
  Device::WIPE_CACHE,
#endif  // !AB_OTA_UPDATER
  Device::WIPER,
  Device::MOUNT_SYSTEM,
  Device::VIEW_RECOVERY_LOGS,
#ifdef RECOVERY_TESTS
  Device::RUN_GRAPHICS_TEST,
  Device::RUN_LOCALE_TEST,
#endif
};

static_assert(sizeof(MENU_ITEMS) / sizeof(MENU_ITEMS[0]) ==
              sizeof(MENU_ACTIONS) / sizeof(MENU_ACTIONS[0]) + 1,
              "MENU_ITEMS and MENU_ACTIONS should have the same length, "
              "except for the extra NULL entry in MENU_ITEMS.");

const char* const* Device::GetMenuItems() {
  return MENU_ITEMS;
}

Device::BuiltinAction Device::InvokeMenuItem(int menu_position) {
  return menu_position < 0 ? NO_ACTION : MENU_ACTIONS[menu_position];
}

int Device::HandleMenuKey(int key, bool visible) {
  if (!visible) {
    return kNoAction;
  }

  switch (key) {
    case KEY_DOWN:
    case KEY_VOLUMEDOWN:
      return kHighlightDown;

    case KEY_UP:
    case KEY_VOLUMEUP:
      return kHighlightUp;

    case KEY_ENTER:
    case KEY_POWER:
      return kInvokeItem;

    default:
      // If you have all of the above buttons, any other buttons
      // are ignored. Otherwise, any button cycles the highlight.
      return ui_->HasThreeButtons() ? kNoAction : kHighlightDown;
  }
}
