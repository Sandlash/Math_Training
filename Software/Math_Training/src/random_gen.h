/*
 * random_gen.h
 *
 *  Created on: 15/mag/2018
 *      Author: emilio
 */

#ifndef RANDOM_GEN_H_
#define RANDOM_GEN_H_

typedef struct {
	int value1, value2, answer;
	char operator;
	char string[7];
}expression;

expression NewExpression();

#endif /* RANDOM_GEN_H_ */
