#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <fstream>

class FileManager {
public:
    static void saveLoginData(const std::string& filename, const std::string& username, const std::string& password);
    static void appendToFile(const std::string& filename, const std::string& data);
}; 


#endif
