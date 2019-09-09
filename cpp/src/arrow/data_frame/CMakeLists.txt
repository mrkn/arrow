# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

add_custom_target(arrow_data_frame)

# Headers: top level
arrow_install_all_headers("arrow/data_set")

# pkg-config support
arrow_add_pkg_config("arrow-data-frame")

set(ARROW_DATA_FRAME_SRCS data_frame.cc)
set(ARROW_DATA_FRAME_LINK_STATIC arrow_static)
set(ARROW_DATA_FRAME_LINK_SHARED arrow_shared)

add_arrow_lib(arrow_data_frame
              OUTPUTS
              ARROW_DATA_FRAME_LIBRARIES
              SOURCES
              ${ARROW_DATA_FRAME_SRCS}
              PRIVATE_INCLUDES
              ${ARROW_DATA_FRAME_PRIVATE_INCLUDES}
              SHARED_LINK_LIBS
              ${ARROW_DATA_FRAME_LINK_SHARED}
              STATIC_LINK_LIBS
              ${ARROW_DATA_FRAME_LINK_STATIC})

if(ARROW_TEST_LINKAGE STREQUAL "static")
  set(ARROW_DATA_FRAME_TEST_LINK_LIBS arrow_data_frame_static ${ARROW_TEST_STATIC_LINK_LIBS})
else()
  set(ARROW_DATA_FRAME_TEST_LINK_LIBS arrow_data_frame_shared ${ARROW_TEST_SHARED_LINK_LIBS})
endif()

foreach(LIB_TARGET ${ARROW_DATA_FRAME_LIBRARIES})
  target_compile_definitions(${LIB_TARGET} PRIVATE ARROW_DS_EXPORTING)
endforeach()

# Adding unit tests part of the "data_frame" portion of the test suite
function(ADD_ARROW_DATA_FRAME_TEST REL_TEST_NAME)
  set(options)
  set(one_value_args PREFIX)
  set(multi_value_args LABELS)
  cmake_parse_arguments(ARG
                        "${options}"
                        "${one_value_args}"
                        "${multi_value_args}"
                        ${ARGN})

  if(ARG_PREFIX)
    set(PREFIX ${ARG_PREFIX})
  else()
    set(PREFIX "arrow-data-frame")
  endif()

  if(ARG_LABELS)
    set(LABELS ${ARG_LABELS})
  else()
    set(LABELS "arrow_data_frame")
  endif()

  if(NOT WIN32)
    add_arrow_test(${REL_TEST_NAME}
                   EXTRA_LINK_LIBS
                   ${ARROW_DATA_FRAME_TEST_LINK_LIBS}
                   PREFIX
                   ${PREFIX}
                   LABELS
                   ${LABELS}
                   ${ARG_UNPARSED_ARGUMENTS})
  endif()
endfunction()

add_arrow_data_frame_test(data_frame_test)
