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

#ifndef ATOM_TOOLS_OUTPUTLOGGER
#define ATOM_TOOLS_OUTPUTLOGGER


#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <ostream>
#include <chrono>
#include <iomanip>
#include <mutex>


namespace Atom
{
    namespace Tools
    {
        class OutputLogger;
    }
}


class Atom::Tools::OutputLogger
{
    public:
        OutputLogger();
        ~OutputLogger(){}

        static bool get_log_to_file() { return log_to_file_; }
        static std::string get_output_file_address() { return output_file_address_; }

        static void set_log_to_file(bool log_to_file) { log_to_file_ = log_to_file; }
        static void set_output_file_address(bool output_file_address) { output_file_address_ = output_file_address; }

        static void Log_(const std::string& message);
        static std::string CurrentDateTime_();

    private:
        static std::mutex mutex_;
        static bool log_to_file_;
        static std::string output_file_address_;
};

#endif // ATOM_TOOLS_OUTPUTLOGGER
