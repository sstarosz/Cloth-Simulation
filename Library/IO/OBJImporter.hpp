#ifndef IO_OBJIMPORTER_HPP
#define IO_OBJIMPORTER_HPP

#include <filesystem>

namespace st::io {
    class ObjImporter {
    public:
        ObjImporter();
        ~ObjImporter();


        void readFromFile(const std::filesystem::path& pathToObjFile);

        void printData() const;
    private:
    };

}



#endif // !IO_OBJIMPORTER_HPP