#!/bin/bash
set -o errexit
set -o nounset

WRAPPER_HOME="$(dirname "$(dirname "$(realpath "${0}")")")"
ANDROID_NDK="${ANDROID_NDK:-"${HOME}/Android/Sdk/ndk/22.0.7026061"}"
TARGET_LLVM_HOME="${TARGET_LLVM_HOME:-"${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64"}"
TARGET_CC="${TARGET_CC:-"${TARGET_LLVM_HOME}/bin/clang.bak"}"
TARGET_CXX="${TARGET_CXX:-"${TARGET_LLVM_HOME}/bin/clang++.bak"}"

reset=$'\x1b[0m'
bold=$'\x1b[1m'
underline=$'\x1b[4m'
red=$'\x1b[31m'

function backup-clang() {
  if [[ ! -e "${TARGET_CC}" ]]; then
    echo "${underline}${TARGET_LLVM_HOME}/bin/clang${reset} => ${underline}${TARGET_CC}${reset}"
    cp "${TARGET_LLVM_HOME}/bin/clang" "${TARGET_CC}"
  else
    echo "${underline}${TARGET_CC}${reset} alreadly backed up"
  fi
  if [[ ! -e "${TARGET_CXX}" ]]; then
    echo "${underline}${TARGET_LLVM_HOME}/bin/clang++${reset} => ${underline}${TARGET_CXX}${reset}"
    cp "${TARGET_LLVM_HOME}/bin/clang++" "${TARGET_CXX}"
  else
    echo "${underline}${TARGET_CXX}${reset} alreadly backed up"
  fi
}

function recover-clang() {
  if [[ -e "${TARGET_CC}" ]]; then
    echo "${underline}${TARGET_CC}${reset} => ${underline}${TARGET_LLVM_HOME}/bin/clang${reset}"
    cp "${TARGET_CC}" "${TARGET_LLVM_HOME}/bin/clang"
  else
    echo "${bold}${red}${TARGET_CC} not found${reset}"
  fi
  if [[ -e "${TARGET_CXX}" ]]; then
    echo "${underline}${TARGET_CXX}${reset} => ${underline}${TARGET_LLVM_HOME}/bin/clang++${reset}"
    cp "${TARGET_CXX}" "${TARGET_LLVM_HOME}/bin/clang"
  else
    echo "${bold}${red}${TARGET_CXX} not found${reset}"
  fi
}

function build() {
  backup-clang
  cmake -S "${WRAPPER_HOME}" -B "${WRAPPER_HOME}/build" --fresh \
    -DCMAKE_BUILD_TYPE=release \
    -DASAN=OFF \
    -DNDK=ON \
    -DLOG_LEVEL=OFF \
    -DTARGET_CC="${TARGET_CC}" \
    -DTARGET_CXX="${TARGET_CXX}"
  make --directory "${WRAPPER_HOME}/build" --jobs
  cp "${WRAPPER_HOME}/build/base" "${TARGET_LLVM_HOME}/bin/clang"
  cp "${WRAPPER_HOME}/build/base++" "${TARGET_LLVM_HOME}/bin/clang++"
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
