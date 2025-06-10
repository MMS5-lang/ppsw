#include <LPC21xx.H>
#include "konwersje.h"
#include "lancuchy.h"

#define MAX_TOKEN_NR 3
#define MAX_KEYWORD_STRING_LTH 10
#define MAX_KEYWORD_NR 3
#define NULL '\0'


enum KeywordCode {LD, ST, RST};

union TokenValue{
	enum KeywordCode eKeyword;
	unsigned int uiNumber;
	char* pcString;
};

enum TokenType {KEYWORD, NUMBER, STRING};

struct Token{
	enum TokenType eType;
	union TokenValue uValue;
};

struct Token asToken[MAX_TOKEN_NR];

unsigned char ucTokenNr;

struct Keyword{
	enum KeywordCode eCode;
	char cString[MAX_KEYWORD_STRING_LTH + 1];
};

struct Keyword asKeywordList[MAX_KEYWORD_NR]={
	{RST, "reset"},
	{LD, "load"},
	{ST, "store"}
};

enum FunctionState{TOKEN, DELIMITER};

unsigned char ucFindTokensInString (char *pcString){
	
	enum FunctionState eState = DELIMITER;
	unsigned char ucTokenNr = 0;
	unsigned char ucCharCounter;
	unsigned char ucCurrentCharacter;
	
	for(ucCharCounter=0;;ucCharCounter++){
		ucCurrentCharacter = pcString[ucCharCounter];
		
		switch (eState){
			
			case TOKEN:
			{
				if(ucTokenNr == MAX_TOKEN_NR){
					return ucTokenNr;
				}	
				else if(ucCurrentCharacter == NULL){
					return ucTokenNr;
				}
				else if(ucCurrentCharacter != ' '){
					eState = TOKEN;
				}
				else{
					eState = DELIMITER;
				}
	      break;
			}
			
			case DELIMITER:
			{
				if(ucCurrentCharacter == NULL){
					return ucTokenNr;
				}
				else if(ucCurrentCharacter == ' '){
					eState = DELIMITER;
				}
				else{
					eState = TOKEN;
					asToken[ucTokenNr].uValue.pcString = pcString + ucCharCounter;
					ucTokenNr++;
				}
				break;
			}
		}
	}
}

enum Result eStringToKeyword (char pcStr[],enum KeywordCode *peKeywordCode){
	
	unsigned char ucKeywordCounter;
	
	for(ucKeywordCounter=0; ucKeywordCounter < MAX_KEYWORD_NR; ucKeywordCounter++){
		if(EQUAL == eCompareString(asKeywordList[ucKeywordCounter].cString, pcStr)){
			*peKeywordCode = asKeywordList[ucKeywordCounter].eCode;
			return OK;
		}
	}
	return ERROR;
}


	
void DecodeTokens(void){

  unsigned char ucTokenCounter;
  struct Token *psCurrentToken;
	char *pcString;
	
  for(ucTokenCounter = 0; ucTokenCounter < ucTokenNr; ucTokenCounter++){
		
    psCurrentToken = &asToken[ucTokenCounter];
		pcString = psCurrentToken->uValue.pcString;
		
		if(OK == eStringToKeyword(pcString, &psCurrentToken->uValue.eKeyword)){
			psCurrentToken->eType = KEYWORD;
		}
		else if(OK == eHexStringToUInt(pcString, &psCurrentToken->uValue.uiNumber)){
			psCurrentToken->eType = NUMBER;
	  }
		else{
			psCurrentToken->eType = STRING;
		}
  }
}

void DecodeMsg(char *pcString){
	
	ucTokenNr = ucFindTokensInString(pcString);
	ReplaceCharactersInString(pcString, ' ', NULL);
	DecodeTokens();
	
}

char pcString[]="reset 0x2 woda_gazowana";
int main(){
	
	
	DecodeMsg(pcString);
	
}

