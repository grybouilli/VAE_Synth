#include <VAE_synth/VAE_synthWidgets.hpp>
#include <VAE_synth/numpyToCppUtils.hpp>

namespace Example
{
void internal_audio::refresh_from(const python::object& module, std::string arr_name)
{
    audio_file.clear(); // clear the file for new data
    auto output_array = python::extract<np::ndarray>(module.attr(arr_name.c_str()))(); // store the np.ndarray into a variable
    copy(output_array, audio_file);
    current_frame=0;
}
}