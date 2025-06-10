#include <LPC21xx.H> // Dolaczenie biblioteki dla mikrokontroler�w LPC21xx, zawiera definicje rejestr�w i funkcji niskopoziomowych.
#include "konwersje.h" // Dolaczenie pliku nagl�wkowego z funkcjami konwersji (np. eHexStringToUInt), uzywanymi w dekodowaniu.
#include "lancuchy.h" // Dolaczenie pliku nagl�wkowego z funkcjami operacji na lancuchach (np. eCompareString, ReplaceCharactersInString).

// Definicja stalej okreslajacej maksymalna liczbe token�w w komunikacie, zgodna ze specyfikacja (max 3 tokeny).
#define MAX_TOKEN_NR 3
// Definicja stalej okreslajacej maksymalna dlugosc slowa kluczowego (10 znak�w) plus miejsce na znak konca lancucha.
#define MAX_KEYWORD_STRING_LTH 10
// Definicja stalej okreslajacej liczbe zdefiniowanych sl�w kluczowych (3: "reset", "load", "store").
#define MAX_KEYWORD_NR 3
// Definicja znaku konca lancucha (NULL), uzywanego do oznaczania konca lancuch�w znak�w.
#define NULL '\0'

// Definicja typu wyliczeniowego KeywordCode, kt�ry reprezentuje kody sl�w kluczowych (LD=load, ST=store, RST=reset).
enum KeywordCode {LD, ST, RST};

// Definicja unii TokenValue, kt�ra pozwala przechowywac r�zne typy wartosci w zaleznosci od typu tokenu.
union TokenValue {
    enum KeywordCode eKeyword; // Pole dla sl�w kluczowych (np. LD, ST, RST), gdy token jest typu KEYWORD.
    unsigned int uiNumber;     // Pole dla liczb (np. 2 z "0x2"), gdy token jest typu NUMBER.
    char* pcString;            // Pole dla lancuch�w (np. "woda_gazowana"), gdy token jest typu STRING.
};

// Definicja typu wyliczeniowego TokenType, kt�ry okresla mozliwe typy token�w w komunikacie.
enum TokenType {KEYWORD, NUMBER, STRING};

// Definicja struktury Token, kt�ra przechowuje informacje o pojedynczym tokenie: jego typ i wartosc.
struct Token {
    enum TokenType eType;      // Pole okreslajace typ tokenu (KEYWORD, NUMBER lub STRING).
    union TokenValue uValue;   // Pole przechowujace wartosc tokenu, zalezna od typu (unia pozwala zaoszczedzic pamiec).
};

// Deklaracja globalnej tablicy struktur Token, kt�ra przechowa do MAX_TOKEN_NR (3) zdekodowanych token�w.
struct Token asToken[MAX_TOKEN_NR];

// Deklaracja globalnej zmiennej przechowujacej liczbe znalezionych token�w w komunikacie.
unsigned char ucTokenNr;

// Definicja struktury Keyword, kt�ra przechowuje informacje o slowie kluczowym: jego kod i lancuch znak�w.
struct Keyword {
    enum KeywordCode eCode;    // Pole przechowujace kod slowa kluczowego (np. RST dla "reset").
    char cString[MAX_KEYWORD_STRING_LTH + 1]; // Pole przechowujace lancuch znak�w slowa kluczowego (np. "reset") z miejscem na NULL.
};

// Deklaracja i inicjalizacja tablicy sl�w kluczowych zgodnie ze specyfikacja (3 slowa: "reset", "load", "store").
struct Keyword asKeywordList[MAX_KEYWORD_NR] = {
    {RST, "reset"},  // Pierwsze slowo kluczowe: kod RST, lancuch "reset".
    {LD, "load"},    // Drugie slowo kluczowe: kod LD, lancuch "load".
    {ST, "store"}    // Trzecie slowo kluczowe: kod ST, lancuch "store".
};

// Definicja typu wyliczeniowego FunctionState, uzywanego w automacie stan�w do wyszukiwania token�w.
enum FunctionState {TOKEN, DELIMITER};

// Definicja funkcji ucFindTokensInString, kt�ra znajduje tokeny w lancuchu i zapisuje ich poczatki w tablicy asToken.
unsigned char ucFindTokensInString(char *pcString) {
    enum FunctionState eState = DELIMITER; // Inicjalizacja stanu poczatkowego automatu na DELIMITER (pomiedzy tokenami).
    unsigned char ucTokenNr = 0;           // Inicjalizacja licznika token�w na 0 (jeszcze zadnego nie znaleziono).
    unsigned char ucCharCounter;           // Deklaracja zmiennej do iteracji po znakach w lancuchu.
    unsigned char ucCurrentCharacter;      // Deklaracja zmiennej przechowujacej biezacy znak z lancucha.

    // Petla nieskonczona (for bez warunku), iteruje po wszystkich znakach w lancuchu az do napotkania NULL.
    for(ucCharCounter = 0; ; ucCharCounter++) {
        ucCurrentCharacter = pcString[ucCharCounter]; // Pobranie biezacego znaku z lancucha na podstawie licznika.

        switch(eState) { // Instrukcja switch zalezna od biezacego stanu automatu (TOKEN lub DELIMITER).
            case TOKEN: // Stan TOKEN: jestesmy wewnatrz tokenu (ciag znak�w niebedacych spacjami).
                if(ucTokenNr == MAX_TOKEN_NR) { // Sprawdzanie, czy osiagnieto maksymalna liczbe token�w (3).
                    return ucTokenNr;           // Jesli tak, zwr�c aktualna liczbe token�w i zakoncz funkcje.
                }
                else if(ucCurrentCharacter == NULL) { // Sprawdzanie, czy napotkano koniec lancucha (NULL).
                    return ucTokenNr;                 // Jesli tak, zwr�c aktualna liczbe token�w i zakoncz funkcje.
                }
                else if(ucCurrentCharacter != ' ') { // Sprawdzanie, czy biezacy znak nie jest spacja.
                    eState = TOKEN;                  // Jesli nie jest spacja, pozostan w stanie TOKEN (kontynuujemy token).
                }
                else {                               // Jesli biezacy znak jest spacja.
                    eState = DELIMITER;              // Przejdz do stanu DELIMITER (koniec tokenu, poczatek odstepu).
                }
                break; // Zakonczenie przypadku TOKEN w switch.

            case DELIMITER: // Stan DELIMITER: jestesmy miedzy tokenami (spacje lub poczatek lancucha).
                if(ucCurrentCharacter == NULL) { // Sprawdzanie, czy napotkano koniec lancucha (NULL).
                    return ucTokenNr;            // Jesli tak, zwr�c aktualna liczbe token�w i zakoncz funkcje.
                }
                else if(ucCurrentCharacter == ' ') { // Sprawdzanie, czy biezacy znak jest spacja.
                    eState = DELIMITER;              // Jesli tak, pozostan w stanie DELIMITER (ciag dalszy odstepu).
                }
                else {                               // Jesli biezacy znak nie jest spacja ani NULL.
                    eState = TOKEN;                  // Przejdz do stanu TOKEN (poczatek nowego tokenu).
                    // Zapisz adres poczatku tokenu (wskaznik na biezacy znak) w tablicy asToken.
                    asToken[ucTokenNr].uValue.pcString = pcString + ucCharCounter;
                    ucTokenNr++;                     // Zwieksz licznik token�w o 1 (znaleziono nowy token).
                }
                break; // Zakonczenie przypadku DELIMITER w switch.
        }
    }
}

// Definicja funkcji eStringToKeyword, kt�ra sprawdza, czy lancuch jest slowem kluczowym i zwraca jego kod.
enum Result eStringToKeyword(char pcStr[], enum KeywordCode *peKeywordCode) {
    unsigned char ucKeywordCounter; // Deklaracja zmiennej do iteracji po liscie sl�w kluczowych.

    // Petla iterujaca po wszystkich zdefiniowanych slowach kluczowych (0 do MAX_KEYWORD_NR-1).
    for(ucKeywordCounter = 0; ucKeywordCounter < MAX_KEYWORD_NR; ucKeywordCounter++) {
        // Por�wnanie podanego lancucha (pcStr) z lancuchem slowa kluczowego z listy (uzywa eCompareString z lancuchy.h).
        if(EQUAL == eCompareString(asKeywordList[ucKeywordCounter].cString, pcStr)) {
            // Jesli lancuchy sa r�wne, zapisz kod slowa kluczowego (np. RST) do zmiennej wskazywanej przez peKeywordCode.
            *peKeywordCode = asKeywordList[ucKeywordCounter].eCode;
            return OK; // Zwr�c OK (sukces), konczac funkcje.
        }
    }
    return ERROR; // Jesli nie znaleziono pasujacego slowa kluczowego, zwr�c ERROR (blad).
}

// Definicja funkcji DecodeTokens, kt�ra dekoduje typ i wartosc kazdego tokenu w tablicy asToken.
void DecodeTokens(void) {
    unsigned char ucTokenCounter;  // Deklaracja zmiennej do iteracji po tokenach w tablicy asToken.
    struct Token *psCurrentToken;  // Deklaracja wskaznika na biezacy token w tablicy asToken.
    char *pcString;                // Deklaracja wskaznika na lancuch znak�w biezacego tokenu.

    // Petla iterujaca po wszystkich znalezionych tokenach (od 0 do ucTokenNr-1).
    for(ucTokenCounter = 0; ucTokenCounter < ucTokenNr; ucTokenCounter++) {
        psCurrentToken = &asToken[ucTokenCounter]; // Pobranie adresu biezacego tokenu z tablicy asToken.
        pcString = psCurrentToken->uValue.pcString; // Pobranie lancucha znak�w tokenu (wczesniej zapisanego jako wskaznik).

        // Sprawdzanie, czy lancuch jest slowem kluczowym, wywolujac eStringToKeyword.
        if(OK == eStringToKeyword(pcString, &psCurrentToken->uValue.eKeyword)) {
            psCurrentToken->eType = KEYWORD; // Jesli tak, ustaw typ tokenu na KEYWORD.
        }
        // Jesli nie jest slowem kluczowym, sprawdz, czy jest liczba szesnastkowa (uzywa eHexStringToUInt z konwersje.h).
        else if(OK == eHexStringToUInt(pcString, &psCurrentToken->uValue.uiNumber)) {
            psCurrentToken->eType = NUMBER; // Jesli tak, ustaw typ tokenu na NUMBER.
        }
        else {                              // Jesli nie jest ani slowem kluczowym, ani liczba.
            psCurrentToken->eType = STRING; // Ustaw typ tokenu na STRING (domyslny typ).
        }
    }
}

// Definicja funkcji DecodeMsg, kt�ra przetwarza caly komunikat, znajdujac i dekodujac tokeny.
void DecodeMsg(char *pcString) {
    // Wywolanie ucFindTokensInString, aby znalezc tokeny w lancuchu i zapisac ich liczbe w ucTokenNr.
    ucTokenNr = ucFindTokensInString(pcString);
    // Zamiana wszystkich spacji w lancuchu na NULL (uzywa ReplaceCharactersInString z lancuchy.h), aby oddzielic tokeny.
    ReplaceCharactersInString(pcString, ' ', NULL);
    // Wywolanie DecodeTokens, aby zdekodowac typ i wartosc kazdego tokenu w tablicy asToken.
    DecodeTokens();
}

// Deklaracja i inicjalizacja przykladowego komunikatu do przetworzenia (zawiera 3 tokeny: "reset", "0x2", "woda_gazowana").
char pcString[] = "reset 0x2 woda";

// Definicja funkcji main, kt�ra jest punktem wejscia programu.
int main() {
    // Wywolanie DecodeMsg dla przykladowego komunikatu, co spowoduje jego zdekodowanie do tablicy asToken.
    DecodeMsg(pcString);
    // Zakonczenie programu (brak return w kodzie zr�dlowym, ale implicite zwraca 0 w C dla main).
}
