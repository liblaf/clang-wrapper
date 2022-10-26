#include "runtime.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../../common/dictionary.h"

#ifndef INTERFACE_COUNTER_PATH
#define INTERFACE_COUNTER_PATH "/sdcard/interface-counter"
#endif  // INTERFACE_COUNTER_PATH

#ifndef FILENAME_CAPACITY
#define FILENAME_CAPACITY 128
#endif  // FILENAME_CAPACITY

#ifndef SAVE_INTERVAL
#define SAVE_INTERVAL 8
#endif  // SAVE_INTERVAL

#ifdef ANDROID
#include <android/log.h>
#define LOG_TAG "liblaf"
#define ALOGV(...) \
  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGF(...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#define ALOGD(...)
#define ALOGI(...)
#define ALOGW(...)
#define ALOGE(...)
#define ALOGF(...)
#endif

DEFINE_DICTIONARY(Dict, const char*, int);

static void save_to_file();
static void __attribute__((constructor(100))) runtime_init();
static void __attribute__((destructor(100))) runtime_exit();

static FILE* file = NULL;
static struct Dict* dict = NULL;
static int counter = 0;

int Dict_cmp(const DictKeyType* a, const DictKeyType* b) {
  return strcmp(*a, *b);
}

int Dict_hash(const DictKeyType* key) {
  int hash = 0;
  for (size_t n = strlen(*key), i = 0; i < n; i++) {
    hash = (hash << 5) | (hash >> 27);
    hash += (*key)[i];
    hash &= 0x7fffffff;
  }
  return hash;
}

void Dict_key_copy(DictKeyType* dest, const DictKeyType* src) { *dest = *src; }

void Dict_value_copy(DictValueType* dest, const DictValueType* src) {
  *dest = *src;
}

void __before_interface_call(const char* loc, const char* called_func_name) {
  int* value = Dict_get(dict, called_func_name);
  if (value) {
    ++(*value);
  } else {
    Dict_put(dict, called_func_name, 1);
  }
  if (++counter >= SAVE_INTERVAL) {
    save_to_file();
    counter = 0;
  }
}

static void save_to_file() {
  rewind(file);
  for (int i = 0; i < Dict_capacity(dict); ++i) {
    if (dict->_buckets[i]) {
      fprintf(file, "%s %d\n", dict->_buckets[i]->key,
              dict->_buckets[i]->value);
    }
  }
  fflush(file);
}

static void runtime_init() {
  if (dict || file) return;
  dict = Dict_new(1000009);
  char filename[FILENAME_CAPACITY];
  sprintf(filename, "%s/%09ld.txt", INTERFACE_COUNTER_PATH, clock());
  file = fopen(filename, "w");
  ALOGI("runtime_init(%s)", filename);
}

static void runtime_exit() {
  ALOGI("runtime_exit()");
  save_to_file();
  fclose(file);
  Dict_delete(dict);
  dict = NULL;
}
