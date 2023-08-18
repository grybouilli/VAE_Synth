cmake_minimum_required(VERSION 3.13 FATAL_ERROR)


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

find_package(Python3 REQUIRED COMPONENTS Interpreter Development NumPy)
if(VERBOSE)
    cmake_print_variables(Python3_INCLUDE_DIRS)
    cmake_print_variables(Python3_NumPy_INCLUDE_DIRS)
endif()
find_package(Boost COMPONENTS python numpy REQUIRED)
target_link_libraries(score_addon_VAE_synth PRIVATE  ${Boost_LIBRARIES} ${Python3_LIBRARIES} ${Python3_NumPy_LIBRARIES})
target_include_directories(score_addon_VAE_synth  PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})

avnd_score_plugin_finalize(
  BASE_TARGET score_addon_VAE_synth
  PLUGIN_VERSION 1
  PLUGIN_UUID "628ef6ec-07d2-49bc-8dee-18db1e1775de"
)
