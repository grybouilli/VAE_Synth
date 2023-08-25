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


namespace python = boost::python;
namespace np = boost::python::numpy;

namespace Example
{
struct internal_audio
{
    void refresh_from(const python::object& module, std::string arr_name);
    std::vector<_Float32> audio_file; // work something out to allow more data types
    int                   current_frame;
};
}