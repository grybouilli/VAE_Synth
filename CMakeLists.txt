cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)
set(CMAKE_PREFIX_PATH "/usr/lib/libtorch/share/cmake/Torch")

if(NOT TARGET score_lib_base)
  include(ScoreExternalAddon)
endif()

if(NOT TARGET score_plugin_avnd)
  return()
endif()

project(score_addon_VAE_synth LANGUAGES CXX)

avnd_score_plugin_init(
  BASE_TARGET score_addon_VAE_synth
)

avnd_score_plugin_add(
  BASE_TARGET score_addon_VAE_synth
  SOURCES
    VAE_synth/VAE_synth.hpp
    VAE_synth/VAE_synthModel.hpp
    VAE_synth/VAE_synthModel.cpp
  TARGET VAE_synth
  MAIN_CLASS VAE_synth
  NAMESPACE Example
)

# set(CMAKE_INSTALL_RPATH "/usr/lib/libtorch")
# set(CMAKE_BUILD_RPATH "/usr/lib/libtorch")
find_package(Torch REQUIRED)
target_link_libraries(score_addon_VAE_synth PRIVATE "${TORCH_LIBRARIES}")

avnd_score_plugin_finalize(
  BASE_TARGET score_addon_VAE_synth
  PLUGIN_VERSION 1
  PLUGIN_UUID "a1a3bb71-aab1-446f-a897-de7eed02c873"
)
