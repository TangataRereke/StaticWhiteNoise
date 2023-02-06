
#include <ctime>
#include <cmath>
#include <iostream>

class Formula{
	public:
		Formula(short unsigned pInputCount);
		~Formula();
		long long getValue(int pParameter1, int pParameter2);
		long long getValue(int pParameter1, int pParameter2, int Parameter3);
		long long getValue(int pParameter1, int pParameter2, int Parameter3, int Parameter4);
		void checkExample(int pParameter1, int pParameter2, int pParameter3, int pTo1, int pTo2, int pTo3);
		void generateFormulaPart();		
	protected:
	private:
		bool areAllValuesUsed();
		void generateFormula();
		enum EQUATION_TYPE{
			EQUATIONTYPE_Add = 0,
			EQUATIONTYPE_Subtraction = 1,
			EQUATIONTYPE_Multiply = 2,
			EQUATIONTYPE_Divide = 3,
			EQUATIONTYPE_Sin = 4,
			EQUATIONTYPE_Log = 5,
			EQUATIONTYPE_Cos = 6,
			EQUATIONTYPE_SquareRoot = 7,
			EQUATIONTYPE_Power = 8
		};
		short unsigned gTotalInputs = 0;

		struct FormulaInput{
			int value = 0;
			bool used = false;
			bool valueChanged = false;
			FormulaInput *next = 0;
		};
		struct FormulaPart{
			long long ivalue = 0;
			FormulaInput *inputValue = 0;
			short unsigned equationType = 0;
			long long valueSoFar = 0;
			FormulaPart *next = 0;
		};

		bool gDoneFirstRun = false;
		bool gInputValuesChanged = true;

		long long getValuePart(FormulaPart *pPart, long long pRunningAmount, bool *pReevaluate);

		FormulaInput *gAllInputs = 0;
		FormulaPart *gAllParts = 0;
		FormulaInput *gLastInput = 0;
		FormulaPart *gLastPart = 0;

		void addInput(FormulaInput *pInput);
		void addFormulaPart(FormulaPart *pPart);
		FormulaInput *getRandomInput();

		long long runFormula();
		long long getValueForPart(FormulaPart *pPart);
};