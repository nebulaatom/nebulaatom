
#include "functions/action.h"
#include "Poco/JSON/Object.h"

using namespace Atom::Functions;

Action::Action(std::string identifier) :
    action_type_(ActionType::kSQL)
    ,identifier_(identifier)
    ,status_("OK.")
    ,message_("OK.")
    ,custom_error_("")
    ,final_(false)
    ,error_(false)
{
    results_ = std::make_shared<Query::Results>();
    json_result_ = new JSON::Object;
}

Action::~Action()
{

}


void Action::IdentifyParameters_()
{
    try
    {
        auto& data_array = get_json_body();

        for (std::size_t a = 0; a < data_array->size(); a++)
        {
            // Get the action object
                if(data_array->get(a).isEmpty())
                {
                    Tools::OutputLogger::instance_.Log_("Data array haves an empty action.");
                    continue;
                }

                auto action_object = data_array->getObject(a);

            // Get the action identifier
                if(action_object->get("action_id").isEmpty() || !action_object->get("action_id").isString())
                {
                    Tools::OutputLogger::instance_.Log_("The action object does not have an action_id String Object.");
                    continue;
                }

                auto action_id = action_object->get("action_id").toString();

            // Verify Action identifier
                if(action_id != get_identifier())
                    continue;

            // Get the parameters object
                if(action_object->get("parameters").isEmpty() || !action_object->get("parameters").isArray())
                {
                    Tools::OutputLogger::instance_.Log_("The action object does not have a parameters array.");
                    continue;
                }

                auto parameters_array = action_object->getArray("parameters");

            // Iterate over parameters array
                for(std::size_t b = 0; b < parameters_array->size(); b++)
                {
                    // Get parameter object
                    if(parameters_array->get(b).isEmpty())
                    {
                        Tools::OutputLogger::instance_.Log_("Parameters array haves an empty element.");
                        continue;
                    }

                    auto parameter_object = parameters_array->getObject(b);

                    // Get parameter name
                    if(parameter_object->get("name").isEmpty() || !parameter_object->get("name").isString())
                    {
                        Tools::OutputLogger::instance_.Log_("Parameter name is not a String Object.");
                        continue;
                    }

                    auto parameter_name = parameter_object->get("name").toString();

                    // Get parameter value
                    if(parameter_object->get("value").isEmpty())
                    {
                        Tools::OutputLogger::instance_.Log_("Parameter value is empty.");
                        continue;
                    }

                    auto parameter_value = parameter_object->get("value");

                    // Find if exists parameter
                    auto found_param = std::find_if(get_parameters().begin(), get_parameters().end(), [parameter_name](Query::Parameter parameter)
                    {
                        return parameter.get_name() == parameter_name;
                    });

                    // Remplace parameter value
                    if(found_param != get_parameters().end())
                    {
                        if(found_param->get_editable())
                        {
                            auto index = std::distance(get_parameters().begin(), found_param);
                            get_parameters().erase(found_param);

                            // Insert new element
                            auto parameter = Query::Parameter(parameter_name, parameter_value, true);
                            get_parameters().insert(get_parameters().begin() + index, std::move(parameter));
                        }
                    }

                }

        }
    }
    catch(std::runtime_error& error)
    {
        std::string string_error = "Error on query_actions.cpp on IdentifyFilters_(): " + std::string(error.what());
        Tools::OutputLogger::instance_.Log_(string_error);
        set_error(true);
        set_custom_error(string_error);
    }
    catch(JSON::JSONException& error)
    {
        std::string string_error = "Error on query_actions.cpp on IdentifyFilters_(): " + std::string(error.what());
        Tools::OutputLogger::instance_.Log_(string_error);
        set_error(true);
        set_custom_error(string_error);
    }
}
