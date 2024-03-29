/*
 * Copyright (C) 2008 The Android Open Source Project
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
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>

#include <cutils/log.h>

#include <linux/input.h>

#include "SensorBase.h"

/*****************************************************************************/

SensorBase::SensorBase(const char *dev_name, const char *data_name)
    : dev_name(dev_name), data_name(data_name), dev_fd(-1), data_fd(-1) {
  if (data_name) {
    data_fd = openInput(data_name);
  }
}

SensorBase::~SensorBase() {
  if (data_fd >= 0) {
    close(data_fd);
  }
  if (dev_fd >= 0) {
    close(dev_fd);
  }
}

int SensorBase::open_device() {
  if (dev_fd < 0 && dev_name) {
    dev_fd = open(dev_name, O_RDONLY);
    ALOGE_IF(dev_fd < 0, "Couldn't open %s (%s)", dev_name, strerror(errno));
  }
  return 0;
}

int SensorBase::close_device() {
  if (dev_fd >= 0) {
    close(dev_fd);
    dev_fd = -1;
  }
  return 0;
}

int SensorBase::write_sys_attribute(const char *path, const char *value,
                                    int bytes) {
  int fd, amt;

  fd = open(path, O_WRONLY);
  if (fd < 0) {
    ALOGE("SensorBase: write_attr failed to open %s (%s)", path,
          strerror(errno));
    return -1;
  }

  amt = write(fd, value, bytes);
  amt = ((amt == -1) ? -errno : 0);
  ALOGE_IF(amt < 0, "SensorBase: write_int failed to write %s (%s)", path,
           strerror(errno));
  close(fd);
  return amt;
}

int SensorBase::getFd() const {
  if (!data_name) {
    return dev_fd;
  }
  return data_fd;
}

int SensorBase::setDelay(int32_t handle, int64_t ns) { return 0; }

int64_t SensorBase::getDelay(int32_t handle) { return 0; }

int SensorBase::populateSensorList(struct sensor_t *list) { return 0; }

void SensorBase::getChipInfo(char *buf) {}
bool SensorBase::hasPendingEvents() const { return false; }

int64_t SensorBase::getTimestamp() {
  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;
  clock_gettime(CLOCK_BOOTTIME, &t);
  return int64_t(t.tv_sec) * 1000000000LL + t.tv_nsec;
}

int SensorBase::openInput(const char *inputName) {
  int fd = -1;
  const char *dirname = "/dev/input";
  char devname[PATH_MAX];
  char *filename;
  DIR *dir;
  struct dirent *de;
  dir = opendir(dirname);
  int path_len = 0;
  if (dir == NULL) return -1;

  snprintf(devname, sizeof(devname), "%s", dirname);  // luciddle
  path_len = strlen(devname);
  filename = devname + path_len;
  *filename++ = '/';
  path_len++;
  while ((de = readdir(dir))) {
    if (de->d_name[0] == '.' &&
        (de->d_name[1] == '\0' ||
         (de->d_name[1] == '.' && de->d_name[2] == '\0')))
      continue;

    snprintf(filename, PATH_MAX - path_len, "%s", de->d_name);  // luciddle
    fd = open(devname, O_RDONLY);
    if (fd >= 0) {
      char name[80];
      if (ioctl(fd, EVIOCGNAME(sizeof(name) - 1), &name) < 1) {
        name[0] = '\0';
      }
      if (strlen(name) > 0 && !strcmp(name, inputName)) {
        snprintf(input_name, PATH_MAX, "%s", filename);  // luciddle
        break;
      } else {
        close(fd);
        fd = -1;
      }
    } else
    ALOGE("open %s fail , fd=%d", devname, fd);
  }
  closedir(dir);
  ALOGE_IF(fd < 0, "couldn't find '%s' input devicei fd=%d", inputName , fd);
  return fd;
}
