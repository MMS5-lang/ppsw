#include <LPC21xx.H>
#include "konwersje.h"

#define NULL '\0'
#define HEX_bm 0x000F

void UIntToHexStr(unsigned int uiValue, char pcStr[]){
	
   unsigned char ucNibbleCounter;
   unsigned char ucCurrentNibble;

   pcStr[0] = '0';
   pcStr[1] = 'x';
   pcStr[6] = NULL;
	
   for(ucNibbleCounter = 0 ; ucNibbleCounter < 4 ; ucNibbleCounter++){
      ucCurrentNibble = (uiValue >> (ucNibbleCounter * 4)) & HEX_bm;
      if(ucCurrentNibble<10){
          pcStr[5 - ucNibbleCounter] = '0' + ucCurrentNibble;
      }
      else
      {
         pcStr[5 - ucNibbleCounter] = 'A' + (ucCurrentNibble - 10);
      }
   }
}


//enum Result {OK, ERROR};
enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue){
	
    unsigned char ucCharacterCounter;
    unsigned char ucCurrentCharacter;
	
    if((pcStr[0] != '0') || (pcStr[1] != 'x') || (pcStr[2] == NULL)){
		return ERROR;
    }
    
    *puiValue = 0;
    
    for(ucCharacterCounter = 2; pcStr[ucCharacterCounter] !=NULL ; ucCharacterCounter++){
			
        ucCurrentCharacter = pcStr[ucCharacterCounter];
			
        if (ucCharacterCounter == 6){
            return ERROR;
        }
				
				*puiValue = *puiValue << 4;
				
        if((ucCurrentCharacter >= '0') && (ucCurrentCharacter <= '9')){
					
            *puiValue = (*puiValue) | (ucCurrentCharacter - '0');
        }
        else if((ucCurrentCharacter >= 'A') && (ucCurrentCharacter <= 'F')){
					
            *puiValue = (*puiValue) | (ucCurrentCharacter - 'A' + 10);
        }
        else
        {
            return ERROR;
        }
    }
    return OK;
}


void AppendUIntToString(unsigned int uiValue, char pcDestinationStr[]){
	
    unsigned char ucArrayEndCounter;

    for(ucArrayEndCounter = 0; pcDestinationStr[ucArrayEndCounter] != NULL; ucArrayEndCounter++){}
    UIntToHexStr(uiValue, &pcDestinationStr[ucArrayEndCounter]);
}


