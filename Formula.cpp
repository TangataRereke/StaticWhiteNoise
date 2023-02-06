#ifndef FORMULA_H
	#define FORMULA_H
	#include "Formula.h"
#endif

long long Formula::getValue(int pValue1, int pValue2){
	FormulaInput *lInput = gAllInputs;
	if(lInput->value!=pValue1){
		lInput->value = pValue1;
		lInput->valueChanged = true;
		gInputValuesChanged = true;
	}else{
		lInput->valueChanged = false;
	}

	lInput = lInput->next;
	if(lInput==0){
		std::cout << "Specified too many inputs for fomrula " << std::endl;
		exit(1);
	}
	if(lInput->value!=pValue2){
		lInput->value = pValue2;
		lInput->valueChanged = true;
		gInputValuesChanged = true;
	}else{
		lInput->valueChanged = false;
	}

	return runFormula();
	
}

long long Formula::getValue(int pValue1, int pValue2, int pValue3){
	FormulaInput *lInput = gAllInputs;
	if(lInput->value!=pValue1){
		lInput->value = pValue1;
		lInput->valueChanged = true;
		gInputValuesChanged = true;
	}else{
		lInput->valueChanged = false;
	}
	lInput = lInput->next;
	if(lInput==0){
		std::cout << "Specified too many inputs for fomrula " << std::endl;
		exit(1);
	}
	if(lInput->value!=pValue2){
		lInput->value = pValue2;
		lInput->valueChanged = true;
		gInputValuesChanged = true;
	}else{
		lInput->valueChanged = false;
	}
	lInput = lInput->next;
	if(lInput==0){
		std::cout << "Specified too many inputs for formula " << std::endl;
		exit(1);
	}
	if(lInput->value!=pValue3){
		lInput->value = pValue3;
		lInput->valueChanged = true;
		gInputValuesChanged = true;
	}else{
		lInput->valueChanged = false;
	}

	return runFormula();
	
}

Formula::Formula(short unsigned pInputCount){
	srand(std::time(nullptr));
	for(int bCount = 1; bCount <= pInputCount; bCount++){
		FormulaInput *lInput = new FormulaInput;
		addInput(lInput);
	}
	generateFormula();
}

Formula::~Formula(){
	FormulaPart *lPart = gAllParts;
	while(lPart){
		FormulaPart *bNext = lPart->next;
		delete lPart;
		lPart = bNext;
	}

	FormulaInput *lInput = gAllInputs;
	while(lInput){
		FormulaInput *bNext = lInput->next;
		delete lInput;
		lInput = bNext;
	}	
}

void Formula::addInput(FormulaInput *pInput){
	gTotalInputs++;

	if(gAllInputs==0){
		gAllInputs = pInput;
		gLastInput = pInput;
		return;
	}
	gLastInput->next = pInput;
	gLastInput = pInput;
}

void Formula::addFormulaPart(FormulaPart *pPart){
	if(gAllParts==0){
		gAllParts = pPart;
		gLastPart = pPart;
		return;
	}
	gLastPart->next = pPart;
	gLastPart = pPart;
}


bool Formula::areAllValuesUsed(){
	FormulaInput *lInput = gAllInputs;
	while(lInput!=0){
		if(!lInput->used){
			return false;
		}
		lInput = lInput->next;
	}
	return true;
}

Formula::FormulaInput *Formula::getRandomInput(){
	short unsigned lPick = rand() % gTotalInputs;
	FormulaInput *lInput = gAllInputs;
	short unsigned lCount = 0;
	while(lInput){
		if(lCount==lPick){
			lInput->used = true;
			return lInput;
		}
		lCount++;
		lInput = lInput->next;
	}
	std::cout << "Get Random input didn't find value " << lPick << " out of " << gTotalInputs << std::endl;
	exit(1);
	return 0;
}

void Formula::checkExample(int pParameter1, int pParameter2, int pParameter3, int pTo1, int pTo2, int pTo3){
	bool lDoneFirst = false;
	int lLastValue = 0;
	for(int bP1 = pParameter1;bP1<=pTo1; bP1++){
		for(int bP2 = pParameter2;bP2<=pTo2; bP2++){
			for(int bP3 = pParameter3;bP3<=pTo3; bP3++){
				short unsigned lValue = abs(getValue(bP1, bP2, bP3)%256);
				if(!lDoneFirst){
					lDoneFirst = true;
				}else{
					if(lLastValue==lValue){
						generateFormulaPart();
						checkExample(pParameter1, pParameter2, pParameter3, pTo1, pTo2, pTo3);
						return;
					}
				}
				lLastValue = lValue;
			}
		}
	}
}

void Formula::generateFormulaPart(){
	short unsigned lType = rand() % 9;
	FormulaPart *lFormulaPart = new FormulaPart;
	lFormulaPart->equationType = lType;

	if(rand()%4==0){
		lFormulaPart->inputValue = getRandomInput();
	}else{
		lFormulaPart->ivalue = rand();
		if(lFormulaPart->ivalue == 0){
			lFormulaPart->ivalue = 1;
		}
		if(rand()%3==0){
			lFormulaPart->ivalue = lFormulaPart->ivalue * -1;
		}
	}
	gDoneFirstRun = false;
	addFormulaPart(lFormulaPart);
}

void Formula::generateFormula(){
	bool lUsedAllInputs = false;
	while(!lUsedAllInputs){
		generateFormulaPart();
		
		if(!lUsedAllInputs){
			lUsedAllInputs = areAllValuesUsed();
		}
	}
}

long long Formula::getValuePart(FormulaPart *pPart, long long pRunningAmount, bool *pReevaluate){
	long long lReturn = pRunningAmount;
	if(!*pReevaluate){
		if(pPart->inputValue&&pPart->inputValue->valueChanged){
			*pReevaluate = true;
		}else{
			return pPart->valueSoFar;
		}
	}
	if(pPart->equationType==EQUATIONTYPE_Add){
		lReturn = lReturn + getValueForPart(pPart);
	}else if(pPart->equationType==EQUATIONTYPE_Cos){
		lReturn = lReturn * cos(getValueForPart(pPart));
	}else if(pPart->equationType==EQUATIONTYPE_Divide){
		lReturn = lReturn / getValueForPart(pPart);
	}else if(pPart->equationType==EQUATIONTYPE_Log){
		lReturn = lReturn * log(getValueForPart(pPart));
	}else if(pPart->equationType==EQUATIONTYPE_Multiply){
		lReturn = lReturn * getValueForPart(pPart);
	}else if(pPart->equationType==EQUATIONTYPE_Power){
		lReturn = pow(lReturn, getValueForPart(pPart));
	}else if(pPart->equationType==EQUATIONTYPE_Sin){
		lReturn = lReturn * sin(getValueForPart(pPart));
	}else if(pPart->equationType==EQUATIONTYPE_SquareRoot){
		lReturn = lReturn + sqrt(getValueForPart(pPart));
	}else if(pPart->equationType==EQUATIONTYPE_Subtraction){
		lReturn = lReturn - getValueForPart(pPart);
	}
	pPart->valueSoFar = lReturn;
	return lReturn;
}

long long Formula::runFormula(){
	if(!gInputValuesChanged&&gLastPart->ivalue!=0){
		return gLastPart->valueSoFar;
	}
	FormulaPart *lPart = gAllParts;
	long long lReturn = 0;
	bool lUpdateValue = !gDoneFirstRun;

	while(lPart){
		lReturn = getValuePart(lPart, lReturn, &lUpdateValue);
		lPart = lPart->next;
	}
	if(!gDoneFirstRun){
		gDoneFirstRun = true;
	}
	gInputValuesChanged = false;
	return lReturn;
}



long long Formula::getValueForPart(FormulaPart *pPart){
	if(pPart->inputValue){
		return pPart->inputValue->value;
	}else{
		return pPart->ivalue;
	}
}