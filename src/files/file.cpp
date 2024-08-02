// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2022 <copyright holder> <email>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "files/file.h"
#include "files/file_properties.h"

using namespace NAF::Files;

NAF::Files::File::File() :
    content_length_(0)
    ,content_type_("")
    ,name_("")
    ,filename_("")
    ,file_type_(FileType::kBinary)
    ,file_properties_("", true, {})
    ,requested_path_(new Path())
    ,requested_file_(new Poco::File(*requested_path_))
    ,tmp_file_(new Poco::File())
{

}

NAF::Files::File::File(std::string name, std::string filename, std::string content_type, std::size_t content_length) :
    content_length_(content_length)
    ,content_type_(content_type)
    ,name_(name)
    ,filename_(filename)
    ,file_type_(FileType::kBinary)
    ,file_properties_("", true, {})
    ,requested_path_(new Path())
    ,requested_file_(new Poco::File(*requested_path_))
    ,tmp_file_(new Poco::File())
{

}
