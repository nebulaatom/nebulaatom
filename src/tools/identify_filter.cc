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

#include "tools/identify_filter.h"

using namespace CPW::Tools;

IdentifyFilter::IdentifyFilter(std::shared_ptr<Tools::Filters> current_filters) :
    current_filters_(current_filters)
{

}

void IdentifyFilter::Fields_(Dynamic::Var& filter)
{

}

void IdentifyFilter::Page_(Dynamic::Var& filter)
{

}

void IdentifyFilter::Limit_(Dynamic::Var& filter)
{

}

void IdentifyFilter::Sort_(Dynamic::Var& filter)
{

}

void IdentifyFilter::Iqual_(Dynamic::Var& filter)
{

}

void IdentifyFilter::NotIqual_(Dynamic::Var& filter)
{

}

void IdentifyFilter::GreatherThan_(Dynamic::Var& filter)
{

}

void IdentifyFilter::SmallerThan_(Dynamic::Var& filter)
{

}

void IdentifyFilter::Between_(Dynamic::Var& filter)
{

}

void IdentifyFilter::In_(Dynamic::Var& filter)
{

}

void IdentifyFilter::NotIn_(Dynamic::Var& filter)
{

}

void IdentifyFilter::Set_(Dynamic::Var& filter)
{

}

void IdentifyFilter::Joins_(Dynamic::Var& filter)
{

}
