/*
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        https://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <QueryOptimizer.hpp>

#include <Plans/LogicalPlan.hpp>
#include <DistributedLogicalPlan.hpp>
#include <Serialization/QueryPlanSerializationUtil.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <google/protobuf/util/json_util.h>

namespace NES
{

void QueryOptimizer::writePlanToJson(const LogicalPlan& plan, const std::string& phase)
{
    auto serialized = QueryPlanSerializationUtil::serializeQueryPlan(plan);
    std::string json;
    auto status = google::protobuf::util::MessageToJsonString(serialized, &json);

    std::string path = phase + ".json";
    std::ofstream file(path);

    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open file: " << path << "\n";
    } else {
        file << json;
        file.close();
        std::cout << "Written to: " << path << "\n";
    }
}

DistributedLogicalPlan QueryOptimizer::optimize(LogicalPlan plan) const
{
    plan = semanticAnalyzer.analyse(plan);
    plan = ruleBasedOptimization.optimize(plan);
    return operatorPlacement.place(plan);
}

}
