#pragma once

/* Needed for boggle.cpp */
#include "grid.h"
#include "lexicon.h"
int scoreBoard(Grid<char>& board, Lexicon& lex);

/* Needed for voting.cpp */
#include "vector.h"

Vector<int> computePowerIndexes(Vector<int>& blocks);
