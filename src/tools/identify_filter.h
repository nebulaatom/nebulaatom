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

#ifndef CPW_TOOLS_IDENTIFYFILTER_H
#define CPW_TOOLS_IDENTIFYFILTER_H


#include <stdexcept>

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>

#include "tools/filters.h"
#include "tools/manage_json.h"


namespace CPW
{
    namespace Tools
    {
        class IdentifyFilter;
    }
}

using namespace Poco;


class CPW::Tools::IdentifyFilter
{
    public:
        IdentifyFilter(std::shared_ptr<Tools::Filters> current_filters);
        ~IdentifyFilter();

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

    protected:
        bool BasicVerification_();
        Tools::ManageJSON manage_json_;

    private:
		std::shared_ptr<Tools::Filters> current_filters_;
};

#endif // CPW_TOOLS_IDENTIFYFILTER_H
