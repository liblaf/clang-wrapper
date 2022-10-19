#!/bin/bash
set -o errexit
set -o nounset

WRAPPER_HOME="$(dirname "$(dirname "$(realpath "${0}")")")"
ANDROID_NDK="${ANDROID_NDK:-"${HOME}/Android/Sdk/ndk/22.0.7026061"}"
TARGET_LLVM_HOME="${TARGET_LLVM_HOME:-"${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64"}"
TARGET_CC="${TARGET_CC:-"${TARGET_LLVM_HOME}/bin/clang.bak"}"
TARGET_CXX="${TARGET_CXX:-"${TARGET_LLVM_HOME}/bin/clang++.bak"}"

function underline() {
  echo -n $'\x1b[4m'
  echo -n "${@}"
  echo $'\x1b[24m'
}

function error() {
  echo -n $'\x1b[1;91m'
  echo -n "${@}"
  echo $'\x1b[0m'
}

function info() {
  echo -n $'\x1b[94m'
  echo -n "${@}"
  echo $'\x1b[0m'
}

function copy() {
  info "$(underline "${1}") => $(underline "${2}")"
  cp "${1}" "${2}"
}

function backup-clang() {
  if [[ ! -e "${TARGET_CC}" ]]; then
    copy "${TARGET_LLVM_HOME}/bin/clang" "${TARGET_CC}"
  else
    info "$(underline "${TARGET_CC}") alreadly backed up"
  fi
  if [[ ! -e "${TARGET_CXX}" ]]; then
    copy "${TARGET_LLVM_HOME}/bin/clang++" "${TARGET_CXX}"
  else
    info "$(underline "${TARGET_CXX}") alreadly backed up"
  fi
}

function recover-clang() {
  if [[ -e "${TARGET_CC}" ]]; then
    copy "${TARGET_CC}" "${TARGET_LLVM_HOME}/bin/clang"
  else
    error "${TARGET_CC} not found"
  fi
  if [[ -e "${TARGET_CXX}" ]]; then
    copy "${TARGET_CXX}" "${TARGET_LLVM_HOME}/bin/clang"
  else
    error "${TARGET_CXX} not found"
  fi
}

function build() {
  backup-clang
  cmake -S "${WRAPPER_HOME}" -B "${WRAPPER_HOME}/build" \
    -DASAN=OFF \
    -DCMAKE_BUILD_TYPE=release \
    -DLOG_LEVEL=OFF \
    -DNDK=ON \
    -DTARGET_CC="${TARGET_CC}" \
    -DTARGET_CXX="${TARGET_CXX}"
  make --directory "${WRAPPER_HOME}/build" --jobs
  copy "${WRAPPER_HOME}/build/base" "${TARGET_LLVM_HOME}/bin/clang"
  copy "${WRAPPER_HOME}/build/base++" "${TARGET_LLVM_HOME}/bin/clang++"
}

case "${1:-"build"}" in
build)
  build
  ;;
backup)
  backup-clang
  ;;
recover)
  recover-clang
  ;;
esac
