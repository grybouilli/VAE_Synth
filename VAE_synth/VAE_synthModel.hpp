#pragma once

#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>
#include <halp/file_port.hpp>

#include <boost/python.hpp> 
#include <boost/python/numpy.hpp>

#include <string>
#include <sstream>
#include <filesystem>
#include <vector>

#include <iostream>

#include "VAE_synthWidgets.hpp"

namespace python = boost::python;
namespace np = boost::python::numpy;

namespace Example
{

    class VAE_synth
    {
    public:
        halp_meta(name, "VAE Synth")
        halp_meta(category, "Audio")
        halp_meta(c_name, "VAE_synth")
        halp_meta(uuid, "14f96468-b70c-4280-8179-460cbc1c1406")

        VAE_synth()
        {
            try
            {
                Py_Initialize(); 
                np::initialize();

                penv.main = python::import("__main__");
                python::exec("import sys; import os", penv.main.attr("__dict__"));

                // Initializes python interpreter the earliest possible to avoid problems
                // idea : put some guards on call of python to allow calling Py_Initialize in prepare?
            }
            catch (const python::error_already_set&)
            {
                PyErr_Print();
            }
        }

        struct internal_audio
        {
            void refresh_from(const python::object& module, std::string arr_name);
            std::vector<_Float32> audio_file; // work something out to allow more data types
            int                   current_frame;
        } internal_audio_data;

        struct python_env
        {
            python::object main;
        } penv;
    
        // USER INPUTS
        struct
        {
            // workspace folder path to allow import of custom modules in python
            halp::lineedit<"Workspace folder", "">  workspace_folder;
            // push button to load workspace folder path
            struct {
                enum widget { pushbutton };
                halp_meta(name, "Enter folder")
                void update(VAE_synth& obj);
                bool value;
            } folder_refresh;
            using folder_refresh_t = decltype(folder_refresh);

            // code editors for python
            // python code editor for model loading
            struct : halp::lineedit<"Program", 
            "import torch                                                       \n"
            "\n"
            "model = None # torch.load your model  \n"
            "if model != None:\n"
            "    print(model)\n"
            "    model.eval()\n"
            >
            {
                halp_meta(language, "Python")
                void update(VAE_synth& obj);
            } model_loader;
            using model_loader_t = decltype(model_loader);

            // python code editor for output treatment
            struct : halp::lineedit<"Program", "">
            {
                halp_meta(language, "Python")
                void update(VAE_synth& obj);
            } program;
            using program_t = decltype(program);

        } inputs;
        using inputs_t = decltype(inputs);
        // !USER INPUTS

        // USER OUTPUTS
        struct {
            halp::fixed_audio_bus<"Output", float, 1> audio;
        } outputs;
        // !USER OUTPUTS

        using setup = halp::setup;
        void prepare(halp::setup info)
        {
        }

        // Do our processing for N samples
        using tick = halp::tick;

        // Defined in the .cpp
        void operator()(halp::tick t);

    };

}
