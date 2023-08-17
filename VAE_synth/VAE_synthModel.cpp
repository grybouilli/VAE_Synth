#include "VAE_synth.hpp"

namespace Example
{
void VAE_synth::operator()(halp::tick t)
{
  // Process the input buffer
    auto & in = inputs.in;
    auto & out = outputs.out;

    if(model_data.model_loaded)
    {
        // inputs for the model
        std::vector<torch::jit::IValue> torch_inputs;
        
        // casting input values in C-array
        float input_val_arr [] = { in.value };
        long inputSize = sizeof(input_val_arr)/sizeof(float);

        // transforming score input flow - with the help of the C-array - into recognizable torch input
        torch::Tensor torch_in = torch::from_blob(input_val_arr, {1, inputSize}).reshape({-1,1}).clone();
        torch_inputs.push_back(torch_in);

        // output spitted by model with given input 
        at::Tensor torch_output = model_data.model.forward(torch_inputs).toTensor();

        out.value = torch_output[0].item().toFloat(); // back to score output flow
    }
}
}
