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
        char * prev_c = getenv("PYTHONPATH");
        std::string previous { prev_c ? prev_c : "" };
        if(previous.find(val) == std::string::npos) // add current path value only once
        {
            std::stringstream newpath {}; 
            newpath << previous << ':' << val;
            std::cerr << "newpath : " << newpath.str() << std::endl;
            setenv("PYTHONPATH", newpath.str().c_str(), 1); // allows python to look into current directory for modules
        }
    }
}

void VAE_synth::inputs_t::model_loader_t::update(VAE_synth& obj)
{
    // this is called every time 'compile' button is hit
    // should setup the environment
    // model should be called 'model'
    try{
        obj.penv.main = python::import("__main__");
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

void VAE_synth::inputs_t::program_t::update(VAE_synth& obj)
{
    // this is called every time 'compile' button is hit
                
}
}
