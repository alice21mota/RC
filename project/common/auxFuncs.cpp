#include "auxFuncs.h"

//Gets file size in bytes
size_t getFileSize(string asset_fname) {
    ifstream file(asset_fname, ios::binary | ios::ate);
    size_t size = file.tellg();
    file.close();
    cout << "Size is " << size << endl;
    return size;
}

string fileSizeString(string asset_fname) {
    
    size_t size = getFileSize(asset_fname);
    ostringstream sizeStr;
    sizeStr << size;

    if (sizeStr.str().size() > 8){
        return "Too many digits";
    }

    return sizeStr.str();
}