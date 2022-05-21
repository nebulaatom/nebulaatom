// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2022 <copyright holder> <email>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CPW_FILES_FILE_H
#define CPW_FILES_FILE_H


#include <string>
#include <memory>

#include "Poco/Path.h"
#include "Poco/File.h"

#include "files/file_properties.h"


namespace CPW
{
    namespace Files
    {
        enum class FileType;
        class File;
    }
}

using namespace Poco;


enum class CPW::Files::FileType
{
    kBinary
    ,kTextPlain
};


class CPW::Files::File
{
    public:
        File();
        File(std::string name, std::string filename, std::string content_type, std::size_t content_length);

        FileType get_file_type() const{return file_type_;}
        std::size_t get_content_length() const{return content_length_;}
        std::string get_content_type() const{return content_type_;}
        std::string get_name() const {return name_;}
        std::string get_filename() const {return filename_;}

		Files::FileProperties* get_file_properties(){return file_properties_;};
		std::shared_ptr<Path>& get_requested_path()
        {
            auto& var = requested_path_;
            return var;
        }
		std::shared_ptr<Poco::File>& get_requested_file()
        {
            auto& var = requested_file_;
            return var;
        }
		std::shared_ptr<Poco::File>& get_tmp_file()
        {
            auto& var = tmp_file_;
            return var;
        }


        void set_file_type(FileType file_type) {file_type_ = file_type;}
        void set_filename(std::size_t content_length) {content_length_ = content_length;}
        void set_content_length(std::size_t content_length) {content_length_ = content_length;}
        void set_content_type(std::string content_type) {content_type_ = content_type;}
        void set_name(std::string name) {name_ = name;}
        void set_filename(std::string filename) {filename_ = filename;}
        void set_file_properties(Files::FileProperties& file_properties){file_properties_ = &file_properties;}

    private:
        std::size_t content_length_;
        std::string content_type_;
        std::string name_;
        std::string filename_;
        FileType file_type_;
        Files::FileProperties* file_properties_;
        std::shared_ptr<Path> requested_path_;
        std::shared_ptr<Poco::File> requested_file_;
        std::shared_ptr<Poco::File> tmp_file_;
};

#endif // CPW_FILES_FILE_H
