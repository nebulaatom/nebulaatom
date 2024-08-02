/*
* <one line to give the program's name and a brief idea of what it does.>
* Copyright (C) 2021  <copyright holder> <email>
*
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

#include "tools/output_logger.h"

using namespace Atom::Tools;

std::mutex OutputLogger::mutex_;
bool OutputLogger::log_to_file_ = false;
bool OutputLogger::print_debug_ = false;
std::string OutputLogger::output_file_address_ = "nebulaatom.log";

OutputLogger::OutputLogger()
{
    
}

void OutputLogger::Log_(Tools::LogType log_type, std::string& message)
{
    if(!print_debug_ && log_type == Tools::LogType::kDebug)
    {
        return;
    }

    mutex_.lock();

    switch(log_type)
    {
        case Tools::LogType::kInfo:message = "[INFO]: " + message; break;
        case Tools::LogType::kWarning: message = "[WARNING]: " + message; break;
        case Tools::LogType::kError: message = "[ERROR]: " + message; break;
        case Tools::LogType::kDebug: message = "[DEBUG]: " + message; break;
    }
    message = CurrentDateTime_() + " " + message;

    std::cout << message << std::endl;

    if (log_to_file_)
    {
        std::ofstream file_stream(output_file_address_, std::ios_base::app);
        if (file_stream.is_open())
        {
            file_stream << message << std::endl;
            file_stream.close();
        }
        else
            std::cerr << CurrentDateTime_() << " Error on output_logger.cpp on Log_(): Error to write to log file" << std::endl;
    }
    mutex_.unlock();
}

void OutputLogger::Log_(std::string message)
{
    Log_(Tools::LogType::kInfo, message);
}

void OutputLogger::Warning_(std::string message)
{
    Log_(Tools::LogType::kWarning, message);
}

void OutputLogger::Error_(std::string message)
{
    Log_(Tools::LogType::kError, message);
}

void OutputLogger::Debug_(std::string message)
{
    Log_(Tools::LogType::kDebug, message);
}

std::string OutputLogger::CurrentDateTime_()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}