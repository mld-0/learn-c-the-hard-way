
//	Ongoing: 2022-08-22T22:45:33AEST continue/break/<?> apply only to loop (not if/case/<ect>) blocks(?)

//	Processing the symbols/keywords of a language is <called> lexical analysis.
//	Said symbols/keywords are lexeme

//	Keywords:
//		auto			Give a local variable a local lifetime
//		break			Exit out of a compound statement
//		case			Branch in switch statement
//		char			Character Datatype
//		const			Unmodifiable variable
//		continue		Continue to the top of a loop
//		default			Default branch in switch statement
//		do				Start do-while loop
//		double			Double floating-point Datatype
//		else			Else-branch of if-statement
//		enum			Define set of int constants
//		extern			Declare a identifier <that-is/as> defined externally
//		float			Floating-point Datatype
//		for				Start for-loop
//		goto			Jump to label
//		if				Start if-statement
//		int				Integer Datatype 
//		long			Long integer Datatype
//		register		Declare a variable to be stored in a CPU register
//		return			Return from function
//		short			Short integer Datatype
//		signed			Signed modifier for integer Datatypes
//		sizeof			Determine size of data
//		static			Preserve variable value after its scope exits
//		struct 			Combine variables into single record
//		switch			Start switch-statement
//		typedef			Create a new type
//		union			Start union-statement
//		unsigned 		Unsigned modifier for integer Datatypes
//		void			Empty Datatype
//		volatile		Declare variable that might be modified elsewhere
//		while			Start while loop

//	if (TEST) {
//		CODE;
//	} else if (TEST) {
//		CODE;
//	} else {
//		CODE;
//	}

//	switch (OPERAND) {
//		case CONSTANT:
//			CODE;
//			break;
//		default:
//			CODE;
//	}

//	while (TEST) {
//		CODE;
//	}

//	for (INIT; TEST; POST) {
//		CODE;
//	}

//	enum { CONST1, CONST2, CONST3 } NAME;

//	if (ERROR) { 
//		goto fail;
//	}
//	fail:
//		CODE;

//	typedef DEFINITION IDENTIFIER;

//	struct NAME {
//		ELEMENTS;
//	} [VARIABLE_NAME];			//	[VARIABLE_NAME] is optional

//	struct is commonly combined with typedef

