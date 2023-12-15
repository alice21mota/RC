#include "filesystem.h"

bool createFolder(filesystem::path directoryPath) {
    if (!filesystem::exists(directoryPath)) {
        if (filesystem::create_directories(directoryPath)) {
            cout << "Directory " << directoryPath << " created successfully." << endl; // Debug
            return true;
        }
        else {
            cerr << "Error creating directory." << endl; // Debug
            return false;
        }
    }
    else {
        cout << "The directory " << directoryPath << " already exists." << endl; // Debug
        return false;
    }
    return true;
}


bool createFile(string path, string content) {
    ofstream file(path);
    if (file.is_open()) {
        if (!content.empty()) {
            file << content;
        }
        file.close();
        cout << "file created in " << path << endl; // Debug
        return true;
    }
    else {
        cerr << "Erro ao criar o ficheiro: " << path << endl; // Debug
        return false;
    }
}

string readFromFile(filesystem::path filePath) {
    // cout << filePath << endl; // Debug
    if (!filesystem::exists(filePath)) {
        cerr << "File don't exists" << endl;
        return "-1"; // FIXME: throw std::runtime_error("Error opening file: " + filePath);
    }
    string content, line;
    ifstream file(filePath);
    if (file.is_open()) {
        while (getline(file, line)) {
            content += line;
        }
        file.close();
        return content;
    }
    else {
        cerr << "Error reading the file: " << filePath << endl; // Debug
        // return -1; // FIXME: throw std::runtime_error("Error opening file: " + filePath);
        return "-1";
    }
}

vector<string> getSortedFilesFromDirectory(filesystem::path directoryPath) {
// vector<filesystem::directory_entry> getSortedFilesFromDirectory(filesystem::path directoryPath) {
    if (filesystem::is_directory(directoryPath)) {
        vector<filesystem::directory_entry> files;
        vector<string> filenames;

        for (const auto& entry : filesystem::directory_iterator(directoryPath)) {
            files.push_back(entry);
        }

        sort(files.begin(), files.end(), [](const filesystem::directory_entry& a, const filesystem::directory_entry& b) {
            return a.path().filename() < b.path().filename();
            });

        for (const auto& entry : files) {
            filenames.push_back(entry.path().filename());
        }

        // return files;
        return filenames;
    }
    else { // FIXME deal with the errors
        cerr << "O caminho especificado não é um diretório válido." << std::endl;
        // exit(-1);
        return vector<string>();
    }
}

bool writeFile(filesystem::path filePath, string fileSize, string fileData) {

    stringstream sstream(fileSize);
    size_t size;
    sstream >> size;

    ofstream outputFile(filePath, ios::binary);
    if (outputFile.is_open()) {
        // Write the file data to the file
        outputFile.write(fileData.data(), static_cast<std::streamsize>(size));

        // Close the file stream
        outputFile.close();

        std::cout << "File \"" << filePath << "\" successfully written to disk." << std::endl;
        return true;
    }
    else {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return false;
    }
}

string getFileData(filesystem::path filePath) {
    string finalBuffer;
    size_t chunkSize = 1024;
    char buffer[chunkSize];

    ifstream file(filePath, ios::binary);

    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return "-1";
    }

    while (!file.eof()) {
        file.read(buffer, chunkSize);

        if (file.gcount() > 0) {
            finalBuffer.append(buffer, file.gcount());
        }
    }

    file.close();

    return finalBuffer;
}