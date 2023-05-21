/*
 * CPW Woodpecker Server
 * Copyright (C) 2021 CPW Online support@cpwonline.org
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

#ifndef CPW_FILTERS_IDENTIFYFILTER_H
#define CPW_FILTERS_IDENTIFYFILTER_H


#include <stdexcept>

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>

#include "filters/filters_manager.h"
#include "tools/manage_json.h"
#include "tools/row_value_formatter.h"


namespace CPW
{
    namespace Filters
    {
        class IdentifyFilter;
    }
}

using namespace Poco;


class CPW::Filters::IdentifyFilter
{
    public:
        IdentifyFilter(std::shared_ptr<FiltersManager> current_filters);
        ~IdentifyFilter();

		std::map<FilterType, std::function<void(Dynamic::Var&)>>& get_filter_type_functors()
        {
            auto& var = filter_type_functors_;
            return var;
        }

        void Fields_(Dynamic::Var& filter);
        void Page_(Dynamic::Var& filter);
        void Limit_(Dynamic::Var& filter);
        void Sort_(Dynamic::Var& filter);
        void Iqual_(Dynamic::Var& filter);
        void NotIqual_(Dynamic::Var& filter);
        void GreatherThan_(Dynamic::Var& filter);
        void SmallerThan_(Dynamic::Var& filter);
        void Between_(Dynamic::Var& filter);
        void In_(Dynamic::Var& filter);
        void NotIn_(Dynamic::Var& filter);
        void Values_(Dynamic::Var& filter);
        void Set_(Dynamic::Var& filter);
        void Joins_(Dynamic::Var& filter);
        void Like_(Dynamic::Var& filter);
        void AS_(Dynamic::Var& filter);
        void Group_(Dynamic::Var& filter);

    protected:
        ValuesProperties GetValue_(Dynamic::Var& var);

    private:
        void MapFilterTypeFunctors_();

        Tools::ManageJSON manage_json_;
        std::shared_ptr<FiltersManager> current_filters_;
        std::map<FilterType, std::function<void(Dynamic::Var&)>> filter_type_functors_;
};

#endif // CPW_FILTERS_IDENTIFYFILTER_H
