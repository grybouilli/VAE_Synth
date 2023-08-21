#include "VAE_synth.hpp"

namespace Example
{
void VAE_synth::operator()(halp::tick t)
{
  // Process the input buffer
    auto & in = inputs.in;
    // auto & out = outputs.audio;
}

void VAE_synth::inputs_t::folder_refresh_t::update(VAE_synth& obj)
{
    auto & val = obj.inputs.workspace_folder.value;
    if(val != "" && std::filesystem::is_directory(val))
    {
        std::stringstream add_new_path_cmd {}; 
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
    // should setup the environment
    // model should be called 'model'
    try{
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
    // this is called every time 'compile' button is hit
                
}
}
