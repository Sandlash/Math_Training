/**
 * @file 1lnn.c
 * @brief Machine learning functionality for a 1-layer neural network
 * @author Matt Lind
 * @date July 2015
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "1lnn.h"
#include "weights_1lnn.h"



/**
 * @details Initialize layer by setting all weights to random values [0-1]
 * @attention It actually makes no difference whether the weights are
 * initialized to a constant (e.g. 0.5) or to a random number.
 * The result (85% success rate) will not change significantly.
 */

void initLayer(Layer *l){
    
    for (int o=0; o<NUMBER_OF_OUTPUT_CELLS; o++){
        
        for (int i=0; i<NUMBER_OF_INPUT_CELLS; i++){
            l->cell[o].input[i]=0;
            l->cell[o].weight[i]=WEIGHTS[o*NUMBER_OF_INPUT_CELLS + i];
        }
        
        l->cell[o].output = 0;
        l->cell[o].bias = 0;
    }
}




/**
 * @details The output prediction is derived by simply sorting all output values
 * and using the index (=0-9 number) of the highest value as the prediction.
 */

uint8_t getLayerPrediction(Layer *l){
    
    double maxOut = 0;
    uint8_t maxInd = 0;
    
    for (int i=0; i<NUMBER_OF_OUTPUT_CELLS; i++){
        if (l->cell[i].output > maxOut){
            maxOut = l->cell[i].output;
            maxInd = i;
        }
    }
    
    return maxInd;
    
}


/**
 * @details Creates an input vector of length NUMBER_OF_INPUT_CELLS
 * of a given MNIST image, setting input vector cells to [0,1]
 * based on the pixels of the image.
 * Scalar pixel intensity [=grey-scale] is ignored, only 0 or 1 [=black-white].
 */

void setCellInput(Cell *c, MNIST_Image *img){
    
    for (int i=0; i<NUMBER_OF_INPUT_CELLS; i++){
        c->input[i] = img->pixel[i] ? 1 : 0;

    }
}


/**
 * @details Calculates a cell's output by suming all input-weight-products
 * and normalizes to [0-1].
 */

void calcCellOutput(Cell *c){
    
    c->output=0;
    
    for (int i=0; i<NUMBER_OF_INPUT_CELLS; i++){
        c->output += c->input[i] * c->weight[i];
    }
    
    c->output /= NUMBER_OF_INPUT_CELLS;             // normalize output (0-1)
}


/**
 * @details Performs the testing of the trained network
 * Same as training a cell, but without updating weights (learning)
 */

void testCell(Cell *c, MNIST_Image *img){
    
    setCellInput(c, img);
    calcCellOutput(c);
    
}



/**
 * @details Tests a layer by looping through and testing its cells
 * Exactly the same as TrainLayer() but WITHOUT LEARNING.
 * @param l A pointer to the layer that is to be training
 */

int testLayer(Layer *l, MNIST_Image *img){

	// loop through all output cells for the given image
	for (int i=0; i < NUMBER_OF_OUTPUT_CELLS; i++){
		testCell(&l->cell[i], img);
	}

	uint8_t predictedNum = getLayerPrediction(l);

    return predictedNum;

}

