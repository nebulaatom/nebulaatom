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

OutputLogger OutputLogger::instance_;
std::ostream* OutputLogger::output_stream_ = nullptr;
bool OutputLogger::log_to_file_ = false;
std::string OutputLogger::output_file_address_ = "nebulaatom.log";

OutputLogger::OutputLogger()
{
    output_stream_ = &std::cout;
}

void OutputLogger::Log_(const std::string& message)
{
    *output_stream_ << CurrentDateTime_() << " " << message << std::endl;

    if (log_to_file_)
    {
        std::ofstream file_stream(output_file_address_, std::ios_base::app);
        if (file_stream.is_open())
        {
            file_stream << CurrentDateTime_() << " " << message << std::endl;
            file_stream.close();
        }
        else
            std::cerr << CurrentDateTime_() << " Error on output_logger.cpp on Log_(): Error to write to log file" << std::endl;
    }
}

std::string OutputLogger::CurrentDateTime_()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}