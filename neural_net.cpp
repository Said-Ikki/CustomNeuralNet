#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream>

struct hidden_layer_node {
    std::vector<float> weights;
};

struct hidden_layer {
    std::vector<hidden_layer_node> layer_weights;
};

float tanh_calc(float value) {
    return tanh(value);
}

float relu_calc(float value) {
    if(value > 0) {
        return value;
    }
    else {
        return 0;
    }
}

float linear_calc(float value) {
    return value;
}

float forward_pass_single_node( std::vector<float> curr_weights, std::vector<float> inputs, float (*func)(float) ) {

    //for(auto k : curr_weights) {
    //    printf("Size of Node: %f", k);
    //}
    //printf("\n");

    if(inputs.size() != curr_weights.size()) { // input no. must equal number of weights in the node
        //std::cout << "Input: " << inputs.size() << " Weights " << curr_weights.size() << std::endl;
        return errno;
    }

    float output = 1.0;
    for(int i = 0; i < curr_weights.size(); i++) { // for all the weights in the node
        //std::cout << "Input: " << inputs.at(i) << " Weight: " << curr_weights.at(i) << " Makes " << inputs.at(i) * curr_weights.at(i) << std::endl;
        output += inputs.at(i) * curr_weights.at(i); // multiply the input with that input's corresponding weight
        // and sum these products together
    }

    output = func(output);
    return output; // one singular output
}

std::vector<float> forward_pass_layer(hidden_layer hl, std::vector<float> inputs, float (*func)(float)) {
    
    std::vector<float> outputs;
    
    for(int i = 0; i < hl.layer_weights.size(); i++) { // for all nodes
        //printf("Size of Layers: %d \n", hl.layer_weights.at(i).weights.size());
        outputs.push_back(forward_pass_single_node( hl.layer_weights.at(i).weights, inputs, func )); // get the node output thing
        //std::cout << outputs.back() << std::endl;
    }

    return outputs;
}

int main() {
    //printf("Hello World!");

    // Key Rules
    //      1. the number of weights per node equals the number of nodes from the previous layer
    //      2. you can go bananas with the number of nodes per layer as long as the weight count is maintained

    std::vector<float> inputs = {1.0, 2.0, 3.0}; // three inputs

    hidden_layer_node h1_1;
    h1_1.weights = {1.0, 2.0, 1.0}; // nodes with 3 weights for 3 inputs
    hidden_layer_node h1_2;
    h1_2.weights = {1.0, 2.0, 1.0};
    hidden_layer_node h1_3;
    h1_3.weights = {1.0, 2.0, 1.0};

    hidden_layer h1;
    h1.layer_weights = {h1_1, h1_2, h1_3}; // 3 nodes in this layer

    hidden_layer_node h2_1;
    h2_1.weights = {1.0, 2.0, 1.0}; // three weights for 3 inputs
    hidden_layer_node h2_2;
    h2_2.weights = {1.0, 2.0, 1.0};
    hidden_layer_node h2_3;
    h2_3.weights = {1.0, 2.0, 1.0};

    hidden_layer h2;
    h2.layer_weights = {h2_1, h2_2}; // only 2 nodes this time

    std::vector<float> outputs = forward_pass_layer(h1, inputs, relu_calc); // inputs to hidden layer 1
    outputs = forward_pass_layer(h2, outputs, tanh_calc); // outputs from previous layer go to hidden layer 2
    std::cout << "Outputs" << std::endl;
    for(auto o : outputs) {
        std::cout << o << std::endl;
    }

    return 0;
}