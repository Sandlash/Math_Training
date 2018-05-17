#include <stdio.h>
#include <stdlib.h>
#include "random_gen.h"
#include <time.h>


expression NewExpression () {
	expression exp;
	int op;

	srand((unsigned)time(NULL));
	op = rand() % 4 + 1;

	switch (op){
		case 1:  // operatore '+'
			exp.operator = '+';
			exp.value1 = rand() % 90;
			do{				   
				exp.value2 = rand() % 90;
				exp.answer = exp.value1 + exp.value2;
			} while (exp.answer < 0 || exp.answer >= 100);
			break;

		case 2:  // operatore '-'
			exp.operator = '-';
			exp.value1 = rand()%70+30;
			do{				   
				exp.value2 = rand()%80+20;
				exp.answer = exp.value1 - exp.value2;
			} while (exp.answer < 0 || exp.answer >= 100);
			break;

		case 3:  // operatore '*'
			exp.operator = '*';
			exp.value1 = rand() % 12 + 1;
			do {
				exp.value2 = rand() % 10;
				exp.answer = exp.value1 * exp.value2;
			} while (exp.answer < 0 || exp.answer >= 100);
			break;

		case 4:  // operatore '%'
			exp.operator = '%';
			exp.value1 = rand() % 80 + 20;
			do {
				exp.value2 = rand() % 90 + 4;
				exp.answer = exp.value1 % exp.value2;
			} while (exp.answer < 0 || exp.answer >100);
			break;

	} //fine switch
	printf("%d %c %d = %d\n",exp.value1, exp.operator, exp.value2, exp.answer );
	return exp;
}
