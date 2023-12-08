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
 * Cria uma pasta no caminho especificado.
 *
 * @param directoryPath O caminho para a pasta a ser criada.
 * @return Retorna 0 se a pasta for criada com sucesso, -1 em caso de erro.
 */
int createFolder(filesystem::path directoryPath);

/**
 * Cria um novo ficheiro no caminho especificado e opcionalmente escreve conteúdo nele.
 *
 * @param path O caminho para o ficheiro a ser criado.
 * @param content O conteúdo a ser escrito no ficheiro (opcional).
 * @return Retorna 0 se o ficheiro for criado com sucesso, -1 em caso de erro.
 */
int createFile(string path, string content = "");

/**
 * Lê o conteúdo de um ficheiro no caminho especificado.
 *
 * @param filePath O caminho para o ficheiro a ser lido.
 * @return Retorna uma string contendo o conteúdo do ficheiro.
 */
string readFromFile(filesystem::path filePath);

/**
 * Obtém uma lista ordenada dos nomes dos ficheiros dentro de uma pasta.
 *
 * @param directoryPath O caminho para a pasta.
 * @return Retorna um vetor de strings ordenadas que contém os nomes dos ficheiros da pasta.
 */
vector<string> getSortedFilesFromDirectory(filesystem::path directoryPath);



#endif