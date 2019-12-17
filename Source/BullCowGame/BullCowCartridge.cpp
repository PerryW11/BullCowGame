// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    VarIsograms = GetIsograms(Words);
    SetupGame();
    // PrintLine(TEXT("The HiddenWord is: %s"), *HiddenWord); // Debug line
    // PrintLine(TEXT("The number of isograms in this game: %i."), GetIsograms(Words).Num());
}
void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::ProcessGuess(const FString &Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You guessed correctly. You Win!"));
        EndGame();
        return;
    }

    PrintLine(TEXT("You have lost a life."));
    PrintLine(TEXT("You have %i lives left."), --Lives);

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You had %i lives left."), Lives);
        PrintLine(TEXT("The hidden word was: %s "), *HiddenWord);
        EndGame();
        return;
    }

    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);


    //Inform the player how many letters the hidden word is
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again!"));
        return;
    }
    if (HiddenWord.Len() != Guess.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long."), HiddenWord.Len());
        return;
    }
}

void UBullCowCartridge::SetupGame()
{
    //Welcome Player
    PrintLine(TEXT("Welcome to Bull Cows!"));

    HiddenWord = VarIsograms[FMath::RandRange(0, VarIsograms.Num() - 1)];
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Enter your guess for the hidden word: "));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}

bool UBullCowCartridge::IsIsogram(const FString &Word) const
{

    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}
TArray<FString> UBullCowCartridge::GetIsograms(const TArray<FString> &Words) const
{
    TArray<FString> Isograms;

    for (FString Word : Words)
    {
        if (IsIsogram(Word))
        {
            Isograms.Emplace(Word);
        }
    }
    return Isograms;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString &Guess) const
{
    FBullCowCount Count;
    for (int32 GuessIndex = 0;  GuessIndex < Guess.Len(); GuessIndex++)
    {
        if(Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if(Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}


