#ifndef MANAGERUSERSFOLDER
#define MANAGERUSERSFOLDER

#include <filesystem>
#include <sstream>
#include <fstream>

#include "filesystem.h"

#include <iostream>
using namespace std;

/**
 * A diretoria USERS contém toda a informação de cada utilizador.
 */
bool createUsersFolder();

/**
 * O servidor AS criará dentro da diretoria USERS uma diretoria para cada utilizador que se registar.
 * A designação da diretoria de utilizador coincide com o UID do utilizador em causa.
 */
bool createUserFolder(string userId);

/**
 * Um ficheiro (uid) pass.txt que contém a password do utilizador.
 * Este ficheiro existirá enquanto o utilizador permanecer registado.
 */
bool createUserPasswordFile(string userId, string password);

/**
 * Um ficheiro (uid) pass.txt que contém a password do utilizador.
 * Este ficheiro existirá enquanto o utilizador permanecer registado.
 */
bool deleteUserPasswordFile(string userId);

/**
 * Um ficheiro (uid) login.txt indicando que o utilizador está em sessão.
 * Este ficheiro existe apenas durante a sessão do utilizador.
 */
bool createUserLoginFile(string userId);

/**
 * Um ficheiro (uid) login.txt indicando que o utilizador está em sessão.
 * Este ficheiro existe apenas durante a sessão do utilizador.
 */
bool deleteUserLoginFile(string userId);

/**
 * Uma diretoria designada HOSTED contendo informação sobre todos os leilões iniciados pelo utilizador.
 * Para cada leilão iniciado pelo utilizador, corresponde um ficheiro dentro da diretoria HOSTED.
 */
bool createHostedFolder(string userId);

/**
 * Uma diretoria designada BIDDED contendo informação sobre todos os leilões nos quais o utilizador licitou.
 * Para cada leilão no qual o utilizador licitou, corresponde um ficheiro dentro da diretoria BIDDED.
 */
bool createBiddedFolder(string userId);

#endif