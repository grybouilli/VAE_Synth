#pragma once

#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>
#include <halp/file_port.hpp>

#include <boost/python.hpp> 
#include <boost/python/numpy.hpp>

#include <string>
#include <string_view>
#include <utility>

enum output_types
{
  audio_file,
  audio_flow,
  OUTPUT_NB
};
template <typename T>
using string_pair = std::pair<std::string_view, T>;
struct output_type_choice
{
  enum widget
  {
    combobox
  };
  static constexpr auto name() { return "Output Type"; }
  struct range
  {
    string_pair<output_types> values[output_types::OUTPUT_NB]{
        {"Audio File", output_types::audio_file},    {"Audio Flow", output_types::audio_flow}};
    output_types init = output_types::audio_file;
  };

  output_types value{};
  int id{};
};