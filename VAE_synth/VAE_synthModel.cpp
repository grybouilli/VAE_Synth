#include "VAE_synth.hpp"
#include "audioUtils.hpp"
#include "numpyToCppUtils.hpp"
namespace Example
{
void VAE_synth::operator()(halp::tick t)
{
    auto& in = internal_audio_data.audio_file;
    auto& out = outputs.audio;
    auto channels = out.channels();
    auto& frames = t.frames;
    auto max_samples = in.size();

    // if we're working with audio file
    if (inputs.outtype_choice.value == output_types::audio_file && max_samples > 0)
    {
        auto& current_frame = internal_audio_data.current_frame;
        current_frame += frames;
        current_frame %= max_samples;

        for (auto i = 0; i < channels; ++i)
        {
            auto& out_samp = out.samples[i];
            copy_sample_into(in, out_samp, frames, current_frame, max_samples);
        }
    }
}

void VAE_synth::inputs_t::folder_refresh_t::update(VAE_synth& obj)
{
    // adds the given path to python path to allow imports of custom modules
    auto& val = obj.inputs.workspace_folder.value;
    if (val != "" && std::filesystem::is_directory(val))
    {
        std::stringstream add_new_path_cmd{};
        add_new_path_cmd << "sys.path.append(\'" << val << "\')\n"
            << "os.chdir(\'" << val << "\')";

        python::exec(
            add_new_path_cmd.str().c_str(),
            obj.penv.main.attr("__dict__")
        );
    }
}

// model loader update function 
void VAE_synth::inputs_t::model_loader_t::update(VAE_synth& obj)
{
    // this is called every time 'compile' button is hit
    try {
        python::exec(
            obj.inputs.model_loader.value.c_str(),
            obj.penv.main.attr("__dict__")
        );
    }
    catch (const python::error_already_set&)
    {
        PyErr_Print();
    }

}

// python code function
void VAE_synth::inputs_t::program_t::update(VAE_synth& obj)
{
    try {
        // execute the program that produces / alter the output
        python::exec(
            obj.inputs.program.value.c_str(),
            obj.penv.main.attr("__dict__")
        );

        // retrieve variable output - which is a np.ndarray - into a c++ vector
        auto& audio_file = obj.internal_audio_data.audio_file;
        if (obj.inputs.outtype_choice.value == output_types::audio_file)
        {
            audio_file.clear(); // clear the file for new data
            auto output_array = python::extract<np::ndarray>(obj.penv.main.attr("output"))(); // store the np.ndarray into a variable
            copy(output_array, audio_file);
        }
        else
        {

        }
        obj.internal_audio_data.current_frame=0;
    }
    catch (const python::error_already_set&)
    {
        PyErr_Print();
    }
}
}
