/*
 * This file is part of Moonlight Embedded.
 *
 * Copyright (C) 2015-2017 Iwan Timmer
 *
 * Moonlight is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moonlight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight; if not, see <http://www.gnu.org/licenses/>.
 */

#include "connection.h"
#include "logging.h"

#include <stdio.h>
#include <stdarg.h>
#include <signal.h>

pthread_t main_thread_id = 0;
bool connection_debug;
ConnListenerRumble rumble_handler = NULL;

static void connection_terminated() {
  if (main_thread_id != 0)
    pthread_kill(main_thread_id, SIGTERM);
}

//TODO: Parse to logging?
static void connection_log_message(const char* format, ...) {
  va_list arglist;
  va_start(arglist, format);
  vprintf(format, arglist);
  va_end(arglist);
}

static void rumble(unsigned short controllerNumber, unsigned short lowFreqMotor, unsigned short highFreqMotor) {
  if (rumble_handler)
    rumble_handler(controllerNumber, lowFreqMotor, highFreqMotor);
}

static void connection_status_update(int status) {
  switch (status) {
    case CONN_STATUS_OKAY:
      _moonlight_log(INFO,"Connection is okay\n");
      break;
    case CONN_STATUS_POOR:
      _moonlight_log(INFO,"Connection is poor\n");
      break;
  }
}

CONNECTION_LISTENER_CALLBACKS connection_callbacks = {
  .stageStarting = NULL,
  .stageComplete = NULL,
  .stageFailed = NULL,
  .connectionStarted = NULL,
  .connectionTerminated = connection_terminated,
  .logMessage = connection_log_message,
  .rumble = rumble,
  .connectionStatusUpdate = connection_status_update
};
