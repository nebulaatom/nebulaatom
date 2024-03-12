/*
 * Nebula Atom
 * Copyright (C) 2021 NebulaCSF support@cpwonline.org
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

#ifndef ATOMFILES_FILEPROPERTIES_H
#define ATOMFILES_FILEPROPERTIES_H


#include <string>
#include <vector>


namespace Atom
{
	namespace Files
	{
		class FileProperties;
	}
}


class AtomFiles::FileProperties
{
	public:
		FileProperties(std::string content_type, bool binary, std::vector<std::string> other_extensions);
		~FileProperties();

		std::string get_content_type()
		{
			auto& var = content_type_;
			return var;
		}
		bool get_binary()
		{
			auto& var = binary_;
			return var;
		}
		std::vector<std::string> get_other_extensions()
		{
			auto& var = other_extensions_;
			return var;
		}

	private:
		std::string content_type_;
		bool binary_;
		std::vector<std::string> other_extensions_;
};

#endif // ATOMFILES_FILEPROPERTIES_H
