include_guard()

function(exec OUTPUT)
  execute_process(COMMAND ${ARGN} OUTPUT_VARIABLE ${OUTPUT})
  string(STRIP ${${OUTPUT}} ${OUTPUT})
  set(${OUTPUT} ${${OUTPUT}} PARENT_SCOPE)
endfunction()

function(llvm_config OUTPUT)
  exec(${OUTPUT} ${LLVM_CONFIG} ${ARGN})
  set(${OUTPUT} ${${OUTPUT}} PARENT_SCOPE)
endfunction()

function(set_default KEY VAL)
  if(NOT ${ARGN} ${KEY})
    set(${KEY} ${VAL} PARENT_SCOPE)
  endif()
endfunction()
