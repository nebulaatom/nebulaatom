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

#ifndef CPW_EXTRAS_INCORPORATEFILTERS_H
#define CPW_EXTRAS_INCORPORATEFILTERS_H


#include <string>
#include <vector>
#include <memory>

#include "tools/filters.h"


namespace CPW
{
	namespace Extras
	{
		class IncorporateFilters;
	}
}


class CPW::Extras::IncorporateFilters
{
	public:
		IncorporateFilters(std::shared_ptr<Tools::Filters> filters);
		~IncorporateFilters();

		void IncorporateWhere_(std::vector<std::string>& tmp_query);
		void IncorporateAND_(std::vector<std::string>& tmp_query);
		void IncorporateFields_(std::vector<std::string>& tmp_query);
		void IncorporatePageLimit_(std::vector<std::string>& tmp_query, bool pagination);
		void IncorporateSort_(std::vector<std::string>& tmp_query);
		void IncorporateIqual_(std::vector<std::string>& tmp_query);
		void IncorporateNotIqual_(std::vector<std::string>& tmp_query);
		void IncorporateGreatherThan_(std::vector<std::string>& tmp_query);
		void IncorporateSmallerThan_(std::vector<std::string>& tmp_query);
		void IncorporateBetween_(std::vector<std::string>& tmp_query);
		void IncorporateIn_(std::vector<std::string>& tmp_query);
		void IncorporateNotIn_(std::vector<std::string>& tmp_query);
		void IncorporateValues_(std::vector<std::string>& tmp_query);
		void IncorporateSet_(std::vector<std::string>& tmp_query);
		void IncorporateJoins_(std::vector<std::string>& tmp_query);

	private:
		std::shared_ptr<Tools::Filters> current_filters_;
};

#endif // CPW_EXTRAS_INCORPORATEFILTERS_H
