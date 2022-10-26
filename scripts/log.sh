#!/bin/bash
set -o errexit
set -o nounset

sub_command="${1:-"purge"}"
shift 1
if [[ -n "${2:-""}" ]]; then
  path="${2}"
else
  WRAPPER_HOME="$(dirname "$(dirname "$(realpath "${0}")")")"
  path="${WRAPPER_HOME}/build/log/"
fi
case "${sub_command}" in
"remove")
  ls "${path}"
  rm --recursive "${path}"
  mkdir --parents "${path}"
  ;;
"purge")
  fd --no-ignore --size -64B . "${path}"
  fd --no-ignore --size -64B . "${path}" --exec rm
  ;;
esac
