## 0.7.0 (2022-10-26)

### Feat

- add JNI Interface Counter
- **runtime**: add utility Dictionary

### Fix

- **wrapper**: manually add definition `ANDROID`
- **pass**: fix compatibility with ASAN
- **common**: seperate log into multiple files
- **common**: fix LOG_LEVEL

### Refactor

- **utility**: refactor file structure

## 0.6.0 (2022-10-19)

### Feat

- **utility**: add location utility
- **common**: remove stdc option when using c++
- **utility**: add JNI Interface Detector
- add assertion utility for debug
- **common**: add path utility for compatibility
- **common**: implement class `Arguments`

### Fix

- **scripts**: pretty output
- **utility**: fix `get_gep_index()`
- **common**: change string vector to `Arguments`

### Refactor

- **runtime**: move files
- **wrapper**: refine code

## 0.5.0 (2022-10-12)

### Feat

- add struct-parser
- **common**: add `no_opaque_pointers` option
- **wrapper**: add `no_opaque_pointers` option

### Fix

- **pass**: fix include error
- **pass**: fix `run_on_instruction()`
- **common**: fix logging args

## 0.4.2 (2022-10-04)

### Perf

- **base**: use reference for const parameter

## 0.4.1 (2022-10-04)

### Fix

- **base**: hide member data from direct access

## 0.4.0 (2022-10-04)

## 0.3.0 (2022-10-04)

### Feat

- **call-hook**: add call-hook wrapper
- **common**: add utility `env`

### Fix

- **base**: fix generated object extension
- **common**: add utility `has_link_stage()`
- **base**: add assertion

### Refactor

- **base**: improve file structure
- **base**: reduce ambiguity
- **base**: improve extensibility
- **base**: improvements

## 0.2.0 (2022-10-04)

### Feat

- **base**: add `PassBase`

### Fix

- **bump**: fix "No tag found to do an incremental changelog"

## 0.1.0 (2022-10-02)

### Feat

- **wrapper**: add common utilities
