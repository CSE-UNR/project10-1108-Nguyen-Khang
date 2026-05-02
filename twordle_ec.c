//Khang Nguyen
//Project 10

#include <stdio.h>
#include <stdbool.h>

#define WORD_LEN   5
#define MAX_GUESSES 6

bool loadWord(char word[]) {
    FILE *fp = fopen("word.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open word.txt\n");
        return false;
    }
    word[0] = '\0';
    fscanf(fp, "%5s", word);
    fclose(fp);
    if (word[0] == '\0') {
        printf("Error: word.txt is empty\n");
        return false;
    }
    return true;
}

void toLower(char str[]) {
    int i;
    for (i = 0; i < WORD_LEN; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + ('a' - 'A');
        }
    }
}

bool isValidGuess(char guess[]) {
    int len = 0;
    int i;

    while (guess[len] != '\0' && guess[len] != '\n') {
        len++;
    }

    if (len != WORD_LEN) {
        printf("Your guess must be exactly %d letters. Try again: ", WORD_LEN);
        return false;
    }

    for (i = 0; i < WORD_LEN; i++) {
        if (!((guess[i] >= 'a' && guess[i] <= 'z') ||
              (guess[i] >= 'A' && guess[i] <= 'Z'))) {
            printf("Your guess may only contain letters. Try again: ");
            return false;
        }
    }

    return true;
}

void getGuess(char guess[]) {
    char buffer[100];

    printf("Enter your guess: ");
    scanf("%99s", buffer);

    while (!isValidGuess(buffer)) {
        scanf("%99s", buffer);
    }

    int i;
    for (i = 0; i < WORD_LEN; i++) {
        guess[i] = buffer[i];
    }
    guess[WORD_LEN] = '\0';

    toLower(guess);
}

void checkCorrectSpot(char guess[], char mystery[], char result[]) {
    int i;
    for (i = 0; i < WORD_LEN; i++) {
        if (guess[i] == mystery[i]) {
            result[i] = guess[i] - ('a' - 'A');
        } else {
            result[i] = guess[i];
        }
    }
    result[WORD_LEN] = '\0';
}

bool isCorrectSpot(char guess[], char mystery[], int pos) {
    return guess[pos] == mystery[pos];
}

bool isInWord(char ch, char mystery[]) {
    int i;
    for (i = 0; i < WORD_LEN; i++) {
        if (ch == mystery[i]) {
            return true;
        }
    }
    return false;
}

void buildHintLine(char guess[], char mystery[], char hintLine[]) {
    int i;
    for (i = 0; i < WORD_LEN; i++) {
        if (!isCorrectSpot(guess, mystery, i) && isInWord(guess[i], mystery)) {
            hintLine[i] = '^';
        } else {
            hintLine[i] = ' ';
        }
    }
    hintLine[WORD_LEN] = '\0';
}

void displayGuesses(char guesses[][WORD_LEN + 1],
                    char hintLines[][WORD_LEN + 1],
                    int numGuesses) {
    int i;
    printf("\n");
    for (i = 0; i < numGuesses; i++) {
        printf("  %s\n", guesses[i]);
        int j;
        bool hasHint = false;
        for (j = 0; j < WORD_LEN; j++) {
            if (hintLines[i][j] == '^') {
                hasHint = true;
                break;
            }
        }
        if (hasHint) {
            printf("  %s\n", hintLines[i]);
        }
    }
    printf("\n");
}

bool checkWin(char result[]) {
    int i;
    for (i = 0; i < WORD_LEN; i++) {
        if (result[i] >= 'a' && result[i] <= 'z') {
            return false;
        }
    }
    return true;
}

void playGame(char mystery[]) {
    char guesses[MAX_GUESSES][WORD_LEN + 1];
    char hintLines[MAX_GUESSES][WORD_LEN + 1];
    char result[WORD_LEN + 1];

    int numGuesses = 0;
    bool won = false;

    printf("Welcome to Twordle!\n");
    printf("Guess the 5-letter word. You have %d tries.\n\n", MAX_GUESSES);

    while (numGuesses < MAX_GUESSES && !won) {
        printf("Guess %d of %d\n", numGuesses + 1, MAX_GUESSES);

        getGuess(guesses[numGuesses]);

        checkCorrectSpot(guesses[numGuesses], mystery, result);
        buildHintLine(guesses[numGuesses], mystery, hintLines[numGuesses]);

        int i;
        for (i = 0; i <= WORD_LEN; i++) {
            guesses[numGuesses][i] = result[i];
        }

        numGuesses++;

        displayGuesses(guesses, hintLines, numGuesses);

        if (checkWin(result)) {
            won = true;
        }
    }

    if (won) {
        printf("You got it in %d guess%s! Great job!\n",
               numGuesses, numGuesses == 1 ? "" : "es");
    } else {
        printf("Better luck next time! The word was: %s\n", mystery);
    }
}

int main(void) {
    char mystery[WORD_LEN + 1];

    if (!loadWord(mystery)) {
        return 1;
    }

    toLower(mystery);
    playGame(mystery);

    return 0;
}




