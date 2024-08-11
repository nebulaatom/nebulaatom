/*
* Nebula Atom

* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "files/file_manager.h"

using namespace NAF::Files;

FileManager::FileManager() :
    operation_type_(OperationType::kDownload)
{
    result_ = new JSON::Object();
    directory_base_ = Tools::SettingsManager::get_basic_properties_().directory_base;
    directory_for_temp_files_ = Tools::SettingsManager::get_basic_properties_().directory_for_temp_files;
}

FileManager::FileManager(FileManager& file_manager)
{
    operation_type_ = file_manager.get_operation_type();
    result_ = file_manager.get_result();
    directory_base_ = file_manager.get_directory_base();
    directory_for_temp_files_ = file_manager.get_directory_for_temp_files();
    files_ = file_manager.get_files();
}

FileManager::FileManager(OperationType operation_type) :
    operation_type_(operation_type)
{
    result_ = new JSON::Object();
    directory_base_ = Tools::SettingsManager::get_basic_properties_().directory_base;
    directory_for_temp_files_ = Tools::SettingsManager::get_basic_properties_().directory_for_temp_files;
}

FileManager::~FileManager()
{

}

void FileManager::handlePart(const MessageHeader& header, std::istream& stream)
{
    Files::File tmp_file;
    Files::File current_file;
    // Get header parameters
        current_file.set_content_type(header.get("Content-Type", "(unspecified)"));
        current_file.set_name(SplitHeaderValue_(header, "Content-Disposition", "name"));

        auto filename = SplitHeaderValue_(header, "Content-Disposition", "filename");
        ReplaceText_(filename, " ", "-");

        current_file.set_filename(filename);
        tmp_file.set_filename(filename);

    // Check temporary file
        if(!ChangePathAndFilename_(tmp_file, directory_for_temp_files_))
            throw std::runtime_error("Could not set file address");

    // Create temporary file
        CountingInputStream istr(stream);
        std::ofstream ostr;
        ostr.open(tmp_file.get_requested_path()->toString());
        StreamCopier::copyStream(istr, ostr);
        ostr.close();

    // Check final target file
        if(!ChangePathAndFilename_(current_file, directory_base_))
            throw std::runtime_error("Could not set file address");

    // Add new file and its temporal file
        current_file.get_requested_file().reset(new Poco::File(*current_file.get_requested_path()));
        current_file.get_tmp_file().reset(new Poco::File(*tmp_file.get_requested_path()));
        files_.push_back(current_file);
}

std::string FileManager::GenerateName_(std::string name)
{
    DateTime now_time;
    Random random_number;

    random_number.seed();
    std::string new_name =
        std::to_string(now_time.year())
        + std::to_string(now_time.month())
        + std::to_string(now_time.day())
        + "_"
        + std::to_string(now_time.hour())
        + std::to_string(now_time.minute())
        + std::to_string(now_time.second())
        + std::to_string(now_time.millisecond())
        + "_"
        + std::to_string(random_number.next())
        + "_"
        + name
    ;
    return new_name;
}

bool FileManager::CheckFile_(Files::File& current_file)
{
    auto& requested_path = current_file.get_requested_path();
    auto& requested_file = current_file.get_requested_file();

    if(requested_path == nullptr || requested_file == nullptr)
        return false;

    switch(operation_type_)
    {
        case OperationType::kDownload:
        case OperationType::kDelete:
        {
            if(requested_file->exists())
            {
                if(requested_file->isDirectory())
                {
                    requested_path->makeDirectory();
                    requested_path->setFileName("index.html");

                    requested_file.reset(new Poco::File(*requested_path));

                    if(!requested_file->exists())
                        return false;
                }
            }
            else
                return false;

            if(!requested_file->canRead())
                return false;

            break;
        }
        case OperationType::kUpload:
        {
            if(requested_file->exists())
                return false;
        }
    }

    return true;
}

bool FileManager::CheckFiles_()
{
    for(auto& file_it : files_)
    {
        if(!CheckFile_(file_it))
            return false;
    }

    return true;
}

bool FileManager::IsSupported_()
{
    for(auto& file_it : files_)
    {
        if(!IsSupported_(file_it))
            return false;
    }

    return true;
}

bool FileManager::IsSupported_(Files::File& file)
{
    if(file.get_requested_path() == nullptr)
        return false;

    std::string extension = file.get_requested_path()->getExtension();
    auto file_found = supported_files_.find(extension);
    if(file_found != supported_files_.end())
    {
        ProcessFiles_(file, file_found->second);
        return true;
    }
    else
    {
        for(auto& it : supported_files_)
        {
            auto extensions = it.second.get_other_extensions();
            auto find_depth = std::find(extensions.begin(), extensions.end(), extension);
            if(find_depth != extensions.end())
            {
                ProcessFiles_(file, it.second);
                return true;
            }
        }
        return false;
    }
}

void FileManager::ProcessFiles_(Files::File& file, Files::FileProperties properties)
{
    file.get_file_properties() = properties;
    file.set_content_type(properties.get_content_type());
    ProcessFileType_();
}

void FileManager::ProcessContentLength_()
{
    for(auto& file_it : files_)
    {
        if(file_it.get_requested_file() == nullptr)
        {
            file_it.set_content_length(0);
            return;
        }

        file_it.set_content_length(file_it.get_requested_file()->getSize());
    }
}

bool FileManager::VerifyMaxFileSize_()
{
    auto max_file_size = Tools::SettingsManager::get_basic_properties_().max_file_size;
    for(auto& file_it : files_)
    {
        int content_length = file_it.get_content_length();
        if(content_length > max_file_size)
            return false;
    }
    return true;
}

void FileManager::DownloadFile_(std::ostream& out_response)
{
    if(files_.empty() || files_.front().get_requested_file()->path() == "")
    {
        out_response << "";
        return;
    }

    std::ifstream requested_file(files_.front().get_requested_file()->path(), std::ios::binary | std::ios::ate);

    std::size_t content_length = requested_file.tellg();
    std::string text_line(content_length, '\0');
    requested_file.seekg(0);

    if(requested_file.read(&text_line[0], content_length))
    {
        out_response << text_line;
    }
    requested_file.close();
}

void FileManager::UploadFile_()
{
    JSON::Array::Ptr array = new JSON::Array();
    for(auto& file_it : files_)
    {
        JSON::Object::Ptr object = new JSON::Object();
        if (!file_it.get_name().empty())
        {
            std::string target = file_it.get_tmp_file()->path();
            std::string destiny = file_it.get_requested_file()->path();

            auto tmp_file = Poco::File(Path(target));
            tmp_file.moveTo(destiny);
            ProcessContentLength_();

            object->set("name", file_it.get_name());
            object->set("filename", file_it.get_requested_path()->getFileName());
            object->set("type", file_it.get_content_type());
            object->set("size", file_it.get_content_length());
        }
        array->set(array->size(), object);
    }
    result_->set("data", array);
}

void FileManager::RemoveFile_()
{
    for(auto& file_it : files_)
        file_it.get_requested_file()->remove();
}

std::string FileManager::SplitHeaderValue_(const MessageHeader& header, std::string header_name, std::string parameter)
{
    if (header.has(header_name))
    {
        std::string disp;
        NameValueCollection params;

        MessageHeader::splitParameters(header[header_name], disp, params);
        return params.get(parameter, "(unnamed)");
    }
    else
        return "";
}

bool FileManager::ChangePathAndFilename_(Files::File& file, std::string directory, bool change_filename)
{
    bool check = false;
    int cont = 0;

    do
    {
        if(change_filename)
            file.get_requested_path().reset(new Path(directory + "/" + GenerateName_(file.get_filename())));
        else
            file.get_requested_path().reset(new Path(directory + "/" + file.get_filename()));
        
        file.get_requested_file().reset(new Poco::File(*file.get_requested_path()));

        check = CheckFile_(file);
        cont++;

        // Condition to avoid infinite loop
        if(cont > 1000000)
            return false;
    }
    while(!check);

    return true;
}

std::size_t FileManager::ReplaceText_(std::string& inout, std::string what, std::string with)
{
    std::size_t count{};

    for
    (
        std::string::size_type pos{}
        ;inout.npos != (pos = inout.find(what.data(), pos, what.length()))
        ;pos += with.length(), ++count
     ){
        inout.replace(pos, what.length(), with.data(), with.length());
    }

    return count;
}

void FileManager::ProcessFileType_()
{
    for(auto& file_it : files_)
    {
        bool file_is_binary = file_it.get_file_properties().get_binary();

        if(file_is_binary)
            file_it.set_file_type(Files::FileType::kBinary);
        else
            file_it.set_file_type(Files::FileType::kTextPlain);
    }
}

void FileManager::AddSupportedFile_(std::string extension, Files::FileProperties file_properties)
{
    supported_files_.emplace(std::make_pair(extension, Files::FileProperties(file_properties)));
}

void FileManager::AddBasicSupportedFiles_()
{
    supported_files_.emplace(std::make_pair("html", Files::FileProperties{"text/html", false, {"htm", "html5"}}));
    supported_files_.emplace(std::make_pair("js", Files::FileProperties{"application/javascript", false, {"js5"}}));
    supported_files_.emplace(std::make_pair("css", Files::FileProperties{"text/css", false, {"css3"}}));
    supported_files_.emplace(std::make_pair("jpeg", Files::FileProperties{"image/jpeg", false, {"jpeg", "jpg"}}));
    supported_files_.emplace(std::make_pair("png", Files::FileProperties{"image/png", true, {""}}));
    supported_files_.emplace(std::make_pair("webp", Files::FileProperties{"image/webp", true, {""}}));
    supported_files_.emplace(std::make_pair("svg", Files::FileProperties{"image/svg+xml", true, {""}}));
    supported_files_.emplace(std::make_pair("ttf", Files::FileProperties{"font/ttf", true, {""}}));
    supported_files_.emplace(std::make_pair("woff", Files::FileProperties{"font/woff", true, {""}}));
    supported_files_.emplace(std::make_pair("woff2", Files::FileProperties{"font/woff2", true, {""}}));
    supported_files_.emplace(std::make_pair("xhtml", Files::FileProperties{"application/xhtml+xml", true, {""}}));
    supported_files_.emplace(std::make_pair("webm", Files::FileProperties{"video/webm", true, {""}}));
    supported_files_.emplace(std::make_pair("xml", Files::FileProperties{"application/xml", true, {""}}));
    supported_files_.emplace(std::make_pair("zip", Files::FileProperties{"application/zip", true, {""}}));
    supported_files_.emplace(std::make_pair("wav", Files::FileProperties{"audio/x-wav", true, {""}}));
    supported_files_.emplace(std::make_pair("pdf", Files::FileProperties{"application/pdf", true, {""}}));
    supported_files_.emplace(std::make_pair("mpeg", Files::FileProperties{"video/mpeg", true, {""}}));
    supported_files_.emplace(std::make_pair("json", Files::FileProperties{"application/json", true, {""}}));
    supported_files_.emplace(std::make_pair("ico", Files::FileProperties{"image/x-icon", true, {""}}));
    supported_files_.emplace(std::make_pair("gif", Files::FileProperties{"image/gif", true, {""}}));
    supported_files_.emplace(std::make_pair("avi", Files::FileProperties{"video/x-msvideo", true, {""}}));
    supported_files_.emplace(std::make_pair("txt", Files::FileProperties{"text/plain", true, {""}}));
}

NAF::Files::File FileManager::CreateTempFile_(std::string uri)
{
    auto tmp_uri = URI(uri);
    auto tmp_file = Files::File("file", Path(tmp_uri.getPath()).getFileName(), "", 0);
    tmp_file.get_requested_path().reset(new Path(directory_base_ + tmp_uri.getPath(), Path::PATH_NATIVE));
    tmp_file.get_requested_file().reset(new Poco::File(*tmp_file.get_requested_path()));
    return tmp_file;
}

NAF::Files::File FileManager::CreateTempFileFromAddress_(std::string address)
{
    auto tmp_uri = URI(address);
    auto tmp_file = Files::File("file", Path(tmp_uri.getPath()).getFileName(), "", 0);
    tmp_file.get_requested_path().reset(new Path(address, Path::PATH_NATIVE));
    tmp_file.get_requested_file().reset(new Poco::File(*tmp_file.get_requested_path()));
    return tmp_file;
}