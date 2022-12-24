#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include "dictionary.h"

#define STATE_SIZE 5
#define STR_BUFF_SIZE 128

/// @brief Inputról érkező karakter átalakítása
/// @return Ha adott karakter 0..9 -> 'd' karakter fogja szimbolizálni, hogy számjegyről van szó
char convert(char* inputChar){
	//bemeneti karakter eleme-e D halmaznak? D = {0, 1, .. ,9}
	if(isdigit(*inputChar))
		return 'd';
	//return 'l' - minden más karakter esetén l-t adjunk vissza, ezt vizsgálva a delta-függvény tudná, hogy 'err' állapotba kell lépnie
	return *inputChar;
}
/// @brief Determinisztikus véges automata implementációja
/// @param input Inputszalag reprezentálása string-gel, erre van felírva balról jobbra az elemzendő szó
/// @param input_size Inputszalag hossza (itt ez véges, Turing-gépnél végtelen lenne)
/// @param rules Szabályokat kulcs-érték-párokként tároló adatszerkezet memóriacíme
/// @return Amennyiben az input szó helyes, úgy 0-val tér vissza, 
///	Egyébként az inputszalag aktuális indexével tér vissza, ahol a hiba előfordult.
unsigned char matches(char input[], size_t input_size, Dictionary* rules) {
	char conv; //átalakított inputról olvasott jel/karakter
	char state[STATE_SIZE] = "s0"; //az automata aktuális állapota = kezdőállapot
	char currStateInput[STATE_SIZE + 2];
	for (size_t i = 0; i < input_size; i++)
	{
		strncpy(currStateInput, state, STATE_SIZE);
		conv = convert(&input[i]); 
		printf("char = %c\n", conv);
		strncat(currStateInput, &conv, 1); //aktuális állapot + átalakított inputról olvasott jel (konkatenált)
		puts(currStateInput);
		Dictionary* foundRule = lookup(currStateInput);
		if(foundRule == NULL){ //ha nem talált szabályt, akkor vége, az automata delta-függvénye nem tudja meghatározni, milyen állapotba is kéne lépnie
			//ekkor parciális esetre futottunk, kilépünk
			puts("Rule not found, exiting...");
			return i + 1; //elakadtam 1., 2., stb. karaktereknél, 0 a sikeres lefutást jelzi
		}
		strncpy(state, foundRule->value, STATE_SIZE);
		printf("Rule found - new state: %s\n", foundRule->value);
	}
	return 0;
}

/// @brief Nyelvtani szabályok feltöltését végzi, ezek a delta-függvény elágazásai, az állapotátmenetek
/// @return kulcs-érték-párok, ahol a kulcsok = állapot és karakter konkatenáltjai, értékek = új állapot
Dictionary* uploadRules(){
	Dictionary* rules;
	rules = put("s0+", "s1");
	rules = put("s0-", "s1");
	rules = put("s0d", "s2");
	rules = put("s2d", "s2");
	rules = put("s1d", "s2");
	//ezzel a két + állapotátmenettel lenne parciálisból teljes a delta-függvény, 
	//tudná, hogy amikor nem számjegy érkezik az inputról, "err" állapotba kell lépnie, így az "err" állapot indikálja az input szó lexikailag hibás
	//rules = put("s0l", "err"); 
	//rules = put("s1l", "err");
	return rules;
}


int main() {
	Dictionary* rules = uploadRules();
	char input[STR_BUFF_SIZE];
	uint8_t result;
	do
	{
		printf("Enter a number: ");
		gets(input);
		result = matches(input, strlen(input), rules);
		printf("%s is%sa number.\n", input, 
			result == 0 ? " " : " NOT ");
		if(result != 0)
			printf("Error found at %d\n", result);
		puts("\nPress 'q' to exit or 'Any' to continue the program...");
	} while (getc(stdin) != 'q');

	free(rules);
	return EXIT_SUCCESS;
}