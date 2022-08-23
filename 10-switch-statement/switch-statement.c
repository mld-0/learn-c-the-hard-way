#include <stdio.h>

//	A switch-statement in C is a jump table.
//	It only supports expressions that result in integers.
//	The program continues running from the case block it reaches. Use 'break' to exit at end of case.
//	Without 'break', the program falls-through - continues running through each subsiquent statement. 

//	Recomended rules:
//		Always include a 'default' case.
//		Document deliberate cases of fall-through
//		Write case/break before writing code for each case.
//		Prefer if-statements

int main(int argc, char* argv[])
{
	char* states[] = { "California", "Oregon", "Washington", "Texas", };
	int len_states = sizeof(states) / sizeof(*states);

	for (int i = 0; i < len_states; ++i) {
		printf("%i: %s\n", i, states[i]);

		for (int j = 0; states[i][j] != '\0'; ++j) {
			char letter = states[i][j];

			switch (letter) {
				case 'a':
				case 'A':
					printf("%d: A\n", j);
					break;
				case 'e':
				case 'E':
					printf("%d: E\n", j);
					break;
				case 'i':
				case 'I':
					printf("%d: I\n", j);
					break;
				case 'o':
				case 'O':
					printf("%d: O\n", j);
					break;
				case 'u':
				case 'U':
					printf("%d: U\n", j);
					break;
				default:
					printf("%d: %c is not a vowel\n", j, letter);
			}

		}

		printf("\n");
	}


	return 0;
}

