#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <stdlib.h>

#define DEFINE_DICTIONARY(Name, _KeyType, _ValueType)                         \
  typedef _KeyType Name##KeyType;                                             \
  typedef _ValueType Name##ValueType;                                         \
                                                                              \
  struct Name##Entry {                                                        \
    Name##KeyType key;                                                        \
    Name##ValueType value;                                                    \
  };                                                                          \
                                                                              \
  struct Name {                                                               \
    struct Name##Entry** _buckets;                                            \
    int _capacity;                                                            \
    int _size;                                                                \
  };                                                                          \
                                                                              \
  int Name##_cmp(const Name##KeyType* a, const Name##KeyType* b);             \
  int Name##_hash(const Name##KeyType* key);                                  \
  void Name##_key_copy(Name##KeyType* dest, const Name##KeyType* src);        \
  void Name##_value_copy(Name##ValueType* dest, const Name##ValueType* src);  \
                                                                              \
  struct Name* Name##_new(const int capacity);                                \
  void Name##_delete(struct Name* self);                                      \
  int Name##_capacity(struct Name* self);                                     \
  Name##ValueType* Name##_get(struct Name* self, const Name##KeyType key);    \
  int Name##_hash(const Name##KeyType* key);                                  \
  void Name##_put(struct Name* self, const Name##KeyType key,                 \
                  const Name##ValueType value);                               \
  int Name##_size(struct Name* self);                                         \
                                                                              \
  int _Name##_probe_for_free(struct Name* self, const Name##KeyType* key);    \
  int _Name##_probe_for_hit(struct Name* self, const Name##KeyType* key);     \
                                                                              \
  struct Name* Name##_new(const int capacity) {                               \
    struct Name* dict = (struct Name*)calloc(1, sizeof(struct Name));         \
    dict->_buckets =                                                          \
        (struct Name##Entry**)calloc(capacity, sizeof(struct Name##Entry*));  \
    dict->_capacity = capacity;                                               \
    dict->_size = 0;                                                          \
    return dict;                                                              \
  }                                                                           \
                                                                              \
  void Name##_delete(struct Name* self) {                                     \
    for (int i = 0; i < self->_capacity; ++i) {                               \
      free(self->_buckets[i]);                                                \
    }                                                                         \
    free(self->_buckets);                                                     \
    free(self);                                                               \
  }                                                                           \
                                                                              \
  int Name##_capacity(struct Name* self) { return self->_capacity; }          \
                                                                              \
  Name##ValueType* Name##_get(struct Name* self, const Name##KeyType key) {   \
    int i = _Name##_probe_for_hit(self, &key);                                \
    if (self->_buckets[i]) {                                                  \
      return &(self->_buckets[i]->value);                                     \
    } else {                                                                  \
      return NULL;                                                            \
    }                                                                         \
  }                                                                           \
                                                                              \
  void Name##_put(struct Name* self, const Name##KeyType key,                 \
                  const Name##ValueType value) {                              \
    int i = _Name##_probe_for_hit(self, &key);                                \
    if (self->_buckets[i]) {                                                  \
      Name##_value_copy(&(self->_buckets[i]->value), &value);                 \
    } else {                                                                  \
      i = _Name##_probe_for_free(self, &key);                                 \
      self->_buckets[i] =                                                     \
          (struct Name##Entry*)calloc(1, sizeof(struct Name##Entry));         \
      Name##_key_copy(&(self->_buckets[i]->key), &key);                       \
      Name##_value_copy(&(self->_buckets[i]->value), &value);                 \
      ++(self->_size);                                                        \
    }                                                                         \
  }                                                                           \
                                                                              \
  int Name##_size(struct Name* self) { return self->_size; }                  \
                                                                              \
  int _Name##_probe_for_free(struct Name* self, const Name##KeyType* key) {   \
    int hash = Name##_hash(key);                                              \
    int i = hash % (self->_capacity);                                         \
    while (self->_buckets[i]) {                                               \
      i = (i + 1) % (self->_capacity);                                        \
    }                                                                         \
    return i;                                                                 \
  }                                                                           \
                                                                              \
  int _Name##_probe_for_hit(struct Name* self, const Name##KeyType* key) {    \
    int hash = Name##_hash(key);                                              \
    int i = hash % (self->_capacity);                                         \
    while (self->_buckets[i] && Name##_cmp(key, &(self->_buckets[i]->key))) { \
      i = (i + 1) % (self->_capacity);                                        \
    }                                                                         \
    return i;                                                                 \
  }

#endif  // DICTIONARY_H_
