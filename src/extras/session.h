/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2023  Jose F Rivas C <email>
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

#ifndef NAF_EXTRAS_SESSION
#define NAF_EXTRAS_SESSION


#include <string>

#include "Poco/Random.h"
#include <Poco/Net/HTTPCookie.h>
#include <Poco/DateTime.h>
#include "Poco/DigestStream.h"
#include "Poco/MD5Engine.h"


namespace NAF
{
    namespace Extras
    {
        class Session;
    }
}

using namespace Poco;
using namespace Poco::Net;


class NAF::Extras::Session
{
    public:
        Session();
        virtual ~Session();

        std::string get_id() const
        {
            return id_;
        }
        std::string get_path() const
        {
            return path_;
        }
        int get_id_user() const
        {
            return id_user_;
        }
        int get_max_age() const
        {
            return max_age_;
        }
        
        void set_id(std::string id)
        {
            id_ = id;
        }
        void set_path(std::string path)
        {
            path_ = path;
        }
        void set_id_user(int id_user)
        {
            id_user_ = id_user;
        }
        void set_max_age(int max_age)
        {
            max_age_ = max_age;
        }

        void GenerateNewSessionID_();

    protected:
        void GenerateSessionID_();
        void GenerateMD5Hash_();

    private:
        std::string id_;
        std::string path_;
        int id_user_;
        int max_age_;
};


#endif // NAF_EXTRAS_SESSION
