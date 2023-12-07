#ifndef FILESYSTEM
#define FILESYSTEM

#include <filesystem>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>


#include <iostream>
using namespace std;

/**
 *
 * @param directoryPath O caminho para a pasta a ser criada.
 * @return Retorna 0 se a pasta for criada com sucesso, -1 em caso de erro.
 *
*/
int createFolder(filesystem::path directoryPath);

/**
 * @param path O caminho para o ficheiro a ser criado.
 * @param content O conteúdo a ser escritop no ficheiro (opcional).
 * @return Retorna 0 se o ficheiro for criado com sucesso, -1 em caso de erro.
 *
*/
int createFile(string path, string content = "");


string readFromFile(filesystem::path filePath);


vector<string> getSortedFilesFromDirectory(filesystem::path directoryPath);



#endif