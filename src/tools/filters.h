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

#ifndef CPW_TOOLS_FILTERS_H
#define CPW_TOOLS_FILTERS_H


#include <string>
#include <map>
#include <vector>

namespace CPW
{
	namespace Tools
	{
		class Filters;
	}
}


class CPW::Tools::Filters
{
	public:
		Filters();
		~Filters();

		std::vector<std::string>& get_fields()
		{
			auto& var = fields_;
			return var;
		}
		std::string get_page() const { return page_; }
		std::string get_limit() const { return limit_; }
		std::vector<std::string>& get_sorts_conditions()
		{
			auto& var = sorts_conditions_;
			return var;
		}
		std::map<std::string, std::string>& get_iquals_conditions()
		{
			auto& var = iquals_conditions_;
			return var;
		}
		std::map<std::string, std::string>& get_not_iquals_conditions()
		{
			auto& var = not_iquals_conditions_;
			return var;
		}
		std::map<std::string, std::string>& get_greather_than()
		{
			auto& var = greather_than_;
			return var;
		}
		std::map<std::string, std::string>& get_smaller_than()
		{
			auto& var = smaller_than_;
			return var;
		}
		std::map<std::string, std::pair<std::string, std::string>>& get_between()
		{
			auto& var = between_;
			return var;
		}
		std::map<std::string, std::vector<std::string>>& get_in()
		{
			auto& var = in_;
			return var;
		}
		std::map<std::string, std::vector<std::string>>& get_not_in()
		{
			auto& var = not_in_;
			return var;
		}
		std::vector<std::vector<std::string>>& get_values()
		{
			auto& var = values_;
			return var;
		}
		std::map<std::string, std::string>& get_set()
		{
			auto& var = set_;
			return var;
		}

		void set_page(std::string page) { page_ = page; }
		void set_limit(std::string limit) { limit_ = limit; }

	private:
		std::vector<std::string> fields_;
		std::string page_;
		std::string limit_;
		std::vector<std::string> sorts_conditions_;
		std::map<std::string, std::string> iquals_conditions_;
		std::map<std::string, std::string> not_iquals_conditions_;
		std::map<std::string, std::string> greather_than_;
		std::map<std::string, std::string> smaller_than_;
		std::map<std::string, std::pair<std::string, std::string>> between_;
		std::map<std::string, std::vector<std::string>> in_;
		std::map<std::string, std::vector<std::string>> not_in_;
		std::vector<std::vector<std::string>> values_;
		std::map<std::string, std::string> set_;
};


#endif // CPW_TOOLS_FILTERS_H
