#include "file_manager.h"
#include <iostream>
#include <fstream>


// добавляет данные в конец файла
void FileManager::appendToFile(const std::string& filename, const std::string& data) {
    std::ofstream outfile(filename, std::ios::app);
    outfile << data << std::endl;
    outfile.close();
}

void FileManager::saveLoginData(const std::string& filename, const std::string& username, const std::string& password) {
    std::ofstream file(filename, std::ios_base::app); // Открыть файл для добавления данных
    if (file.is_open()) {  // Проверка, успешно ли открылся файл.
        file << "Username: " << username << ", Password: " << password << std::endl; // Запись данных пользователя в файл. Данные форматируются для удобства чтения.

        file.close();
    }
    else {
        std::cerr << "Unable to open file " << filename << " for writing." << std::endl;
        throw std::runtime_error("Unable to open file for writing.");
    }
}
