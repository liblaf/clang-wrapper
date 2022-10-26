#include "dictionary.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

DEFINE_DICTIONARY(Dict, int, char);

int Dict_cmp(const DictKeyType* a, const DictKeyType* b) { return *a - *b; }

int Dict_hash(const DictKeyType* key) { return *key % 512; }

void Dict_key_copy(DictKeyType* dest, const DictKeyType* src) { *dest = *src; }

void Dict_value_copy(DictValueType* dest, const DictValueType* src) {
  *dest = *src;
}

void test_dict(int n) {
  struct Dict* dict = Dict_new(n);
  for (int i = 0; i < (n / 2); ++i) {
    switch (rand() % 2) {
      case 0: {  // get
        DictKeyType key = rand() % 10000;
        printf("Searching %d(%d): ", key, Dict_hash(&key));
        DictValueType* value = Dict_get(dict, key);
        if (value) {
          printf("Found with %c\n", *value);
        } else {
          printf("Not found\n");
        }
        break;
      }
      case 1: {
        DictKeyType key = rand() % 10000;
        DictValueType value = 'A' + (rand() % 26);
        printf("Inserting <%d(%d),%c>\n", key, Dict_hash(&key), value);
        Dict_put(dict, key, value);
        break;
      }
      default: {
        abort();
      }
    }
  }
  Dict_delete(dict);
  dict = NULL;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage: %s <size of test>\n", argv[0]);
    return 1;
  }
  srand((unsigned int)time(NULL));
  test_dict(atoi(argv[1]));
  return 0;
}
