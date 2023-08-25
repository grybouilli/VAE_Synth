#include "VAE_synthModel.hpp"
#include "audioUtils.hpp"
#include "numpyToCppUtils.hpp"

void exec_code(const std::string& code, python::object module)
{
    try {
        python::exec(
            code.c_str(),
            module.attr("__dict__")
        );
    }
    catch (const python::error_already_set&)
    {
        PyErr_Print();
    }
}
namespace Example
{

// adds the given path to python path to allow imports of custom modules
void VAE_synth::inputs_t::folder_refresh_t::update(VAE_synth& obj)
{
    std::string folder_path = obj.inputs.workspace_folder.value;
    if (folder_path != "" && std::filesystem::is_directory(folder_path))
    {
        std::stringstream add_new_path_cmd{};
        add_new_path_cmd << "sys.path.append(\'" << folder_path << "\')\n"
            << "os.chdir(\'" << folder_path << "\')";

        python::exec(
            add_new_path_cmd.str().c_str(),
            obj.penv.main.attr("__dict__")
        );
    }
}

void VAE_synth::inputs_t::model_loader_t::update(VAE_synth& obj)
{
    // this is called every time 'compile' button is hit
    exec_code(obj.inputs.model_loader.value, obj.penv.main);
}

// python code function
void VAE_synth::inputs_t::program_t::update(VAE_synth& obj)
{
    exec_code(obj.inputs.program.value, obj.penv.main);
    try {
        // retrieve variable output - which is a np.ndarray - into a c++ vector
        obj.internal_audio_data.refresh_from(obj.penv.main, "output");
    }
    catch (const python::error_already_set&)
    {
        PyErr_Print();
    }
}

void VAE_synth::operator()(halp::tick t)
{

    auto& in = internal_audio_data.audio_file;
    auto& out = outputs.audio;
    auto channels = out.channels();
    auto& frames = t.frames;
    auto max_samples = in.size();

    // if we're working with audio file
    if (max_samples > 0)
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
}
