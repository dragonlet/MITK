#-----------------------------------------------------------------------------
# SOFA
#-----------------------------------------------------------------------------

if(MITK_USE_SOFA)
  # Sanity checks
  if(DEFINED SOFA_DIR AND NOT EXISTS ${SOFA_DIR})
    message(FATAL_ERROR "SOFA_DIR variable is defined but corresponds to non-existing directory")
  endif()

  function(write_downloadfile_script script_filename remote local md5)
    file(WRITE ${script_filename}
"message(STATUS \"downloading...
     src='${remote}'
     dst='${local}'\")

file(DOWNLOAD
  \"${remote}\"
  \"${local}\"
  SHOW_PROGRESS
  EXPECTED_MD5 ${md5}
  STATUS status
  LOG log)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
  message(FATAL_ERROR \"error: downloading '${remote}' failed
     status_code: \${status_code}
     status_string: \${status_string}
     log: \${log}\")
endif()

message(STATUS \"downloading... done\")")
  endfunction()

  function(write_extractfile_script script_filename filename directory)
    set(args "")

    if(filename MATCHES "(\\.|=)(bz2|tar\\.gz|tgz|zip)$")
      set(args "xfz")
    elseif(filename MATCHES "(\\.|=)tar$")
      set(args "xf")
    else()
      message(SEND_ERROR "error: do not know how to extract '${filename}' -- known types are .bz2, .tar, .tar.gz, .tgz, and .zip")
      return()
    endif()

    file(WRITE ${script_filename}
"get_filename_component(filename \"${filename}\" ABSOLUTE)
get_filename_component(directory \"${directory}\" ABSOLUTE)

message(STATUS \"extracting...
     src='\${filename}'
     dst='\${directory}'\")

if(NOT EXISTS \"\${filename}\")
  message(FATAL_ERROR \"error: file to extract does not exists: '\${filename}'\")
endif()

set(i 0)
while(EXISTS \"\${directory}/../tmp-\${i}\")
  math(EXPR i \"\${i} + 1\")
endwhile()
set(ut_dir \"\${directory}/../tmp-\${i}\")
file(MAKE_DIRECTORY \"\${ut_dir}\")

message(STATUS \"extracting... [tar ${args}]\")
execute_process(COMMAND \${CMAKE_COMMAND} -E tar ${args} \${filename} WORKING_DIRECTORY \${ut_dir} RESULT_VARIABLE rv)

if(NOT rv EQUAL 0)
  message(STATUS \"extracting... [error clean up]\")
  file(REMOVE_RECURSE \"\${ut_dir}\")
  message(FATAL_ERROR \"error: extract of '\${filename}' failed\")
endif()

message(STATUS \"extracting... [copy]\")
file(GLOB contents \"\${ut_dir}/*\")
file(COPY \${contents} DESTINATION \${directory})

message(STATUS \"extracting... [clean up]\")
file(REMOVE_RECURSE \"\${ut_dir}\")

message(STATUS \"extracting... done\")")
  endfunction()

  set(proj SOFA)
  set(proj_DEPENDENCIES)
  set(SOFA_DEPENDS ${proj})

  set(stamp_dir "${CMAKE_BINARY_DIR}/${proj}-cmake/src/${proj}-stamp")
  set(download_dir "${CMAKE_BINARY_DIR}/${proj}-cmake/src")
  set(source_dir "${CMAKE_BINARY_DIR}/${proj}-src")

  set(url_base "http://dl.dropbox.com/u/5822501/")
  set(rev "8935")
  set(file "sofa-rev${rev}.zip")
  set(md5 "ec5028b9b3ec23293983c4d785325847")

  write_downloadfile_script(
    "${stamp_dir}/download-sofa.cmake"
    "${url_base}${file}"
    "${download_dir}/${file}"
    "${md5}"
  )

  write_extractfile_script(
    "${stamp_dir}/extract-sofa.cmake"
    "${download_dir}/${file}"
    "${source_dir}"
  )

  set(download_cmd ${CMAKE_COMMAND} -P ${stamp_dir}/download-sofa.cmake)
  list(APPEND download_cmd ${CMAKE_COMMAND} -P ${stamp_dir}/extract-sofa.cmake)

  set(file "sofa-rev${rev}-patch.zip")
  set(md5 "34709f71ad134502ca2cec68737f1883")

  write_downloadfile_script(
    "${stamp_dir}/download-patch.cmake"
    "${url_base}${file}"
    "${download_dir}/${file}"
    "${md5}"
  )

  write_extractfile_script(
    "${stamp_dir}/extract-patch.cmake"
    "${download_dir}/${file}"
    "${source_dir}"
  )

  list(APPEND download_cmd ${CMAKE_COMMAND} -P ${stamp_dir}/download-patch.cmake)
  list(APPEND download_cmd ${CMAKE_COMMAND} -P ${stamp_dir}/extract-patch.cmake)

  set(file "sofa-rev${rev}-cmake.zip")
  set(md5 "bc49343e153ff0eec563d20dc0bbd91f")

  write_downloadfile_script(
    "${stamp_dir}/download-cmake.cmake"
    "${url_base}${file}"
    "${download_dir}/${file}"
    "${md5}"
  )

  write_extractfile_script(
    "${stamp_dir}/extract-cmake.cmake"
    "${download_dir}/${file}"
    "${source_dir}"
  )

  list(APPEND download_cmd ${CMAKE_COMMAND} -P ${stamp_dir}/download-cmake.cmake)
  list(APPEND download_cmd ${CMAKE_COMMAND} -P ${stamp_dir}/extract-cmake.cmake)

  if(NOT DEFINED SOFA_DIR)
    ExternalProject_Add(${proj}
      SOURCE_DIR ${source_dir}
      BINARY_DIR ${proj}-build
      PREFIX ${proj}-cmake
      DOWNLOAD_COMMAND "${download_cmd}"
      INSTALL_COMMAND ""
      CMAKE_GENERATOR ${gen}
      CMAKE_ARGS
        ${ep_common_args}
        ${additional_cmake_args}
      DEPENDS ${proj_DEPENDENCIES}
    )

    set(SOFA_DIR ${CMAKE_CURRENT_BINARY_DIR}/${proj}-build)
  else()
    mitkMacroEmptyExternalProject(${proj} "${proj}_DEPENDENCIES}")
  endif()
endif()
