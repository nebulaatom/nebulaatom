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

#include "extras/session.h"

using namespace AtomExtras;

Session::Session() :
    id_("")
    ,path_("/")
    ,user_("")
    ,max_age_(3600)
{
    GenerateSessionID_();
}

Session::~Session()
{

}

void Session::GenerateNewSessionID_()
{
    GenerateSessionID_();
}

void Session::GenerateSessionID_()
{
    for(int a = 0; a < 32; a++)
    {
        Random random;
        random.seed();
        id_.push_back(random.nextChar());
    }

    GenerateMD5Hash_();
}

void Session::GenerateMD5Hash_()
{
	MD5Engine md5;
	DigestOutputStream ostr(md5);
	ostr << id_;
	ostr.flush();
	const DigestEngine::Digest& digest = md5.digest();
	id_ = DigestEngine::digestToHex(digest);
}
