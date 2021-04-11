#ifndef RESOURCEe_FILES_MANAGER_H
#define RESOURCEe_FILES_MANAGER_H

#include <map>
#include <vector>
#include <memory>
#include <string>


namespace resource_files {

    struct FilePtr {

        FilePtr(char *_start, char *_end);

        FilePtr() = default;

        char *start;
        char *end;
    };

    class ResourceFilesGetter {
    private:
        std::map<std::string, FilePtr> file_pointers;

        void setFilePointers();

        ResourceFilesGetter();

        ResourceFilesGetter(const ResourceFilesGetter &) = delete;

        ResourceFilesGetter(ResourceFilesGetter &&) = delete;

        ResourceFilesGetter &operator=(ResourceFilesGetter &) = delete;

    public:

        static std::shared_ptr<ResourceFilesGetter> getInstance();

        std::vector<std::string> getFileNames();

        bool getFilePtr(const std::string &file_name, FilePtr &file);
    };


}
#endif /* RESOURCEe_FILES_MANAGER_H */
