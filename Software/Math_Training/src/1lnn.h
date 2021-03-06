/**
 * @file 1lnn.h
 * @brief Machine learning functionality for a 1-layer neural network
 * @author Matt Lind
 * @date July 2015
 */
#ifndef LNN_H_
#define LNN_H_

#include <stdio.h>
#include <stdint.h>

#define MNIST_IMG_WIDTH 28                                  ///< image width in pixel
#define MNIST_IMG_HEIGHT 28                                 ///< image height in pixel

#define NUMBER_OF_INPUT_CELLS 784   ///< use 28*28 input cells (= number of pixels per MNIST image)
#define NUMBER_OF_OUTPUT_CELLS 10   ///< use 10 output cells to model 10 digits (0-9)

#define LEARNING_RATE 0.05          ///< Incremental increase for changing connection weights



typedef struct Cell Cell;
typedef struct Layer Layer;
typedef struct Vector Vector;
typedef struct MNIST_Image MNIST_Image;



/**
 * @brief Data block defining a MNIST image
 * @see http://yann.lecun.com/exdb/mnist/ for details
 */
struct MNIST_Image{
    uint8_t pixel[28*28];
};




/**
 * @brief Core unit of the neural network (neuron and synapses)
 */

struct Cell{
    double input [NUMBER_OF_INPUT_CELLS];
    double weight[NUMBER_OF_INPUT_CELLS];
    double output;
    double bias;
};




/**
 * @brief The single (output) layer of this network (a layer is number cells)
 */

struct Layer{
    Cell cell[NUMBER_OF_OUTPUT_CELLS];
};




/**
 * @brief Data structure containing defined number of integer values (the output vector contains values for 0-9)
 */

struct Vector{
    int val[NUMBER_OF_OUTPUT_CELLS];
};




/**
 * @brief Returns an output vector with targetIndex set to 1, all others to 0
 * @param targetIndex Index of the output that is to be set to 1
 */

Vector getTargetOutput(int targetIndex);




/**
 * @brief Initialize layer by setting all weights to random values [0-1]
 * @param l A pointer to a NN layer
 */

void initLayer(Layer *l);




/**
 * @brief Returns the index of the cell with the hightest output
 * @param l A pointer to a NN layer
 */

uint8_t getLayerPrediction(Layer *l);




/**
 * @brief Sets a cell's input according to the pixels of a given MNIST image
 * @param c A pointer to a cell
 * @param img A pointer to an MNIST image
 */

void setCellInput(Cell *c, MNIST_Image *img);




/**
 * @brief Calculates a cell's output by suming all input-weight-products
 * @param c A cell of a NN layer
 */

void calcCellOutput(Cell *c);




/**
 * @brief Returns the difference between a target value and the cell's ouput
 * @param c The cell whose output is to be compared
 * @param target The desired value (= correct answer in supervised learning)
 */

double getCellError(Cell *c, int target);




/**
 * @brief Updates a cell's weights based on given error and LEARNING_RATE
 * @param c The cell whose weights are to be updated.
 * @param err The error (difference between desired output and actual output
 */

void updateCellWeights(Cell *c, double err);




/**
 * @brief Performs the training algorithm
 * @param c Pointer to the cell that is to be trained
 * @param img Pointer to the image that is to be processed
 * @param target Desired output value
 */

void trainCell(Cell *c, MNIST_Image *img, int target);




/**
 * @brief Performs the testing of a single cell of the network
 * @param c Pointer to the cell that is to be trained
 * @param img Pointer to the image that is to be processed
 */


void testCell(Cell *c, MNIST_Image *img);


/**
 * @brief Performs the testing of the trained network
 * @param c Pointer to the cell that is to be trained
 * @param img Pointer to the image that is to be processed
 */
int testLayer(Layer *l, MNIST_Image *img);


#endif
