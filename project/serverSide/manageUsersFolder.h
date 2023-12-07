#ifndef MANAGEDIRECTORIES
#define MANAGEDIRECTORIES

#include <filesystem>
#include <sstream>
#include <fstream>

#include "filesystem.h"

#include <iostream>
using namespace std;

/**
 * Diretoria USERS contém toda a informação de cada utilizador
*/
void createUsersFolder();

/**
 * O servidor AS criar´a dentro da directoria USERS uma directoria por cada utilizador que se regista.
 * A designa¸c˜ao da directoria de utilizador coincide com o UID do utilizador em causa.
*/
void createUserFolder(string userId);

/**
 * Um ﬁcheiro (uid) pass.txt que cont´em a password do utilizador.
 * Este ﬁcheiro existir´a enquanto o utilizador permanecer registado.
 */
bool createUserPasswordFile(string userId, string password);

/**
 * Um ﬁcheiro (uid) pass.txt que cont´em a password do utilizador.
 * Este ﬁcheiro existir´a enquanto o utilizador permanecer registado.
 */
bool deleteUserPasswordFile(string userId);

/**
 * Um ﬁcheiro (uid) login.txt indicando que o utilizador está em sessão.
 * Este ﬁcheiro existe apenas durante a sess˜ao do utilizador
*/
void createUserLoginFile(string userId);

/**
 *
 * Um ﬁcheiro (uid) login.txt indicando que o utilizador está em sessão.
 * Este ﬁcheiro existe apenas durante a sess˜ao do utilizador
*/
bool deleteUserLoginFile(string userId);

/**
 * Uma directoria designada HOSTED contendo informa¸c˜ao sobre todos os leil˜oes iniciados pelo utilizador.
 * A cada leil˜ao iniciado pelo utilizador corresponde um ficheiro dentro da directoria HOSTED.
*/
void createHostedFolder(string userId);

/**
 * Uma directoria designada BIDDED contendo informa¸c˜ao sobre todos os leil˜oes nos quais o utilizador licitou. A cada leil˜ao no qual o
 * utilizador licitou, corresponde um ﬁcheiro dentro da directoria BIDDED
*/
void createBiddedFolder(string userId);

#endif