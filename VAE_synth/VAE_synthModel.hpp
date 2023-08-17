#pragma once

#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>
#include <halp/file_port.hpp>
#include <ATen/core/ivalue.h>
#include <torch/script.h>
#include <iostream>

namespace Example
{

    class VAE_synth
    {
    public:
        halp_meta(name, "Pytorch VAE Synth Addon")
        halp_meta(category, "Audio")
        halp_meta(c_name, "VAE_synth")
        halp_meta(uuid, "14f96468-b70c-4280-8179-460cbc1c1406")

        // Define inputs and outputs ports.
        // See the docs at https://github.com/celtera/avendish
        struct model_info
        {
            torch::jit::script::Module  model;
            bool                        model_loaded;
        } model_data;

        struct
        {
            struct {
                static consteval auto name() { return "in"; }
                float value;
            } in;
            struct : halp::file_port<"Traced model file", halp::mmap_file_view> {
                void update(VAE_synth& obj) {
                    obj.model_data.model_loaded = obj.load_model();
                }
            } model_file;

            struct : halp::lineedit<"Program", "">
            {
                halp_meta(language, "Python")

                void update(VAE_synth& obj)
                {
                    std::cerr << "in program : " << obj.inputs.program.value << std::endl;
                }
            } program;

        } inputs;

        struct
        {
            struct {
                static consteval auto name() { return "out"; }
                float value;
            } out;
        } outputs;

        using setup = halp::setup;
        void prepare(halp::setup info)
        {
            model_data.model_loaded = false;
            model_data.model_loaded = load_model();
        }
        
        bool load_model()
        {
            std::string filename { inputs.model_file.file.filename };
            try
            {
                // Deserialize the Scriptmodel from a file using torch::jit::load().
                model_data.model = torch::jit::load(filename);
            }
            catch (const c10::Error& e)
            {
                std::cerr << "error loading the model : " << filename << "\n";
                return false;
            }

            std::cout << "Module : " << filename << " loaded\n";

            return true;
        }

        // Do our processing for N samples
        using tick = halp::tick;

        // Defined in the .cpp
        void operator()(halp::tick t);

    };

}
