#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(
  const std::string& layout, //orig. pattern
  const std::string& floatingLeft, //floating letters that are left and have to be placed
  const std::set<string>& possibleWords, //holds all the possible words 
  const std::set<string>& prefixes, //for more pruning
  std::string currWord, //current word 
  int pos, //index of the current position 
  std::set<string>& sol //holds potential solutions 
);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> results; //for solutions 

    std::set<std::string> filtDict; //to filter the dictionary using the length of the word 
    std::set<std::string>::const_iterator it;

    for (it = dict.begin(); it != dict.end(); ++it) {
      if((int)it->size() == (int)in.size()) {
        filtDict.insert(*it); 
      }
    }
    //for prefixes 
    std::set<std::string> prefixes; 
    std::set<std::string>::const_iterator pit;

    for (pit = filtDict.begin(); pit != filtDict.end(); ++pit) { //
      if((int)pit->size() == (int)in.size()) {
        const std::string& w = *pit; //to store the full word 
        int length = (int)w.size(); //get length 
        for(int i=1; i < length; ++i){ //get all prefixes 
          prefixes.insert(w.substr(0,i)); //insert 
        }
      }
    }

    wordleHelper(in, floating, filtDict, prefixes, "", 0, results); 
    return results; 
}

// Define any helper functions here
void wordleHelper( 
  const std::string& layout, //orig. layout (pattern of fixed letters)
  const std::string& floatingLeft, //floating letters that are left and have to be placed
  const set<string>& possibleWords, //holds all the possible words 
  const std::set<string>& prefixes, //for more pruning
  std::string currWord, //current word 
  int pos, //index of the current position 
  std::set<string>& sol //holds potential solutions 
)
{
  int lSize = layout.size(); //to hold the size 

  //base case 
  if (pos == lSize) {
    //check to see if all floating letters were used and it is a possible word 
    if(floatingLeft.empty() && possibleWords.find(currWord) != possibleWords.end()) {
      sol.insert(currWord); 
    }
    return; 
  }

  //not enough pos left for floating letters 
  if (lSize - pos < static_cast<int>(floatingLeft.size())) {
    return; 
  }

  if (!currWord.empty()) {
    if (prefixes.find(currWord) == prefixes.end()){
      return; 
    }
  }
  //1st case --> in this pos there is a fixed char  
  char layoutChar = layout[pos]; //char in current position in layout
  if (layoutChar != '-') {
    //move floating letters that are left (might remove)
    std::string newFloating = floatingLeft; 
    //if char is one of floating letters than remove 1 of that floating letter from list 
    for (int i=0; i < (int)newFloating.size(); ++i) {
      if (newFloating[i] == layoutChar) {
        newFloating.erase(i,1); //erase
        break; //for only one 
      }
    }

    //recurse to next pos with fixed char added in 
    wordleHelper(
      layout, 
      newFloating, //updated 
      possibleWords, 
      prefixes,
      currWord + layoutChar, //add fixed char to current word 
      pos + 1, //next index
      sol
    );
    return; 
  } else {
    int left = lSize - pos;
    //if left == number of floating letter that are left then every pos must be one of floating letter 
    if (left == (int)floatingLeft.size()) {
      //go through each floating letter 
      for (int i = 0; i < (int)floatingLeft.size(); ++i) {
        //remove 
        std::string newFloating = floatingLeft;
        newFloating.erase(i, 1);

        wordleHelper(
        layout,
        newFloating, 
        possibleWords,
        prefixes,
        currWord + floatingLeft[i],
        pos + 1, 
        sol 
      ); 
      }
      return; 
    } else {

    //2nd case --> has '-' --> choose any letter 
    //go through a - z for the position that is blank 
    for (char i = 'a'; i <= 'z'; ++i) {
      std::string newFloating = floatingLeft; //starts at current floating 
      //if i is floating letter, remove 1 (used) 
      for (int j = 0; j < (int)newFloating.size(); ++j){
        if(newFloating[j] == i) {
          newFloating.erase(j, 1); //erase
          break; 
        }
      }
      wordleHelper(
        layout,
        newFloating,
        possibleWords,
        prefixes,
        currWord + i,
        pos + 1,
        sol
      );
    }
    return; 
    }
  }
}