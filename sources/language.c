/**
 * @file language.c
 * @author HERZBERG Dwayne and BERLIN Florian
 * @brief Function for system language management
 * @version 0.1
 * @date 2022-02-21
 */

#include "../headers/language.h"

/**
 * @brief Get the Langue object
 * 
 * @return Language 
 */
Language GetLangue(){
    return EN;
}

/**
 * @brief return the text corresponding to the requested designation, in the agreed language.
 * 
 * @param var 
 * @return char const* 
 */
char const *LangGet(STR_NAME var){
    return strings[var].text[GetLangue()];
};