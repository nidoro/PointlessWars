#include "DebugServices.h"

DebugServices* DebugServices::instance = nullptr;

DebugServices::DebugServices() : clockDebugStart(std::clock()),
                                 outputStream(std::cout.rdbuf()),
                                 errorOutputStream(std::cout.rdbuf()),
                                 maxStoredTimedBlockEntries(30000),
                                 maxStoredWatchedVariableEntries(30000),
                                 maxStoredMessages(30000),
                                 maxShowedWatchedVariableEntries(100),
                                 maxShowedTimedBlockEntries(100),
                                 maxShowedMessages(100){

}

DebugServices& DebugServices::get(){
    if (!instance) instance = new DebugServices;
    return *instance;
}

void DebugServices::initialize(sf::RenderWindow* window){
    this->window = window;
}

void DebugServices::showUI(){
    ImGui::Begin("Debug Services");
        elapsedTime = (isPaused ? elapsedTime : diffclock(std::clock(), clockDebugStart));
        ImGui::PushItemWidth(200);
        ImGui::Text("Elapsed time:");
        ImGui::SameLine();
        ImGui::Text("%.2fs", elapsedTime);
        ImGui::SameLine();
        std::string label = (isPaused ? "Unpause":"Pause");
        if (ImGui::Button(label.c_str())){
            isPaused = !isPaused;
        };
        ImGui::PopItemWidth();
/*
        if (ImGui::TreeNode("Filters")){
            ImGui::Button("Clear all");
            ImGui::PushItemWidth(100);
            ImGui::Text("Show instant:");
            ImGui::SameLine();
            ImGui::InputFloat("##float1", &instant);
            ImGui::SameLine();
            ImGui::Button("Clear##1");
            ImGui::Text("Show interval:");
            ImGui::SameLine();
            ImGui::SameLine();
            ImGui::InputFloat("##float2", &beginInterval);
            ImGui::SameLine();
            ImGui::InputFloat("##float3", &endInterval);
            ImGui::SameLine();
            ImGui::Button("Clear##2");
            ImGui::Text("Show function:");
            ImGui::SameLine();
            ImGui::InputText("##text1", functionFilter, 1000);
            ImGui::SameLine();
            ImGui::Button("Clear##1");
            ImGui::PopItemWidth();
            ImGui::TreePop();
        }
*/

        if (ImGui::TreeNode("Messages log")){
            for (auto& p : messagesLists){
                if (ImGui::TreeNode(p.first.c_str())){
                    for (auto& i : p.second){
                        ImGui::Text("%s -- %s: %s", i.fullAddress.c_str(), i.type.c_str(), i.message.c_str());
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Watched Variables")){
            for (auto& p : watchedVariableRecords){
                if (ImGui::TreeNode(p.first.c_str())){
                    int showCount = 0;
                    std::list<WatchedVariableRecord>::iterator it = p.second.end();
                    while (it != p.second.begin()){
                        it--;
                        WatchedVariableRecord& i = *it;
                        ImGui::PushItemWidth(60);
                        ImGui::Text("%s = %s (at instant %.4fs)", i.name.c_str(), i.value.c_str(), i.timeStamp);
                        ImGui::PopItemWidth();
                        if (++showCount >= maxShowedWatchedVariableEntries) break;
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Timed Blocks")){
            for (auto& p : timedBlocksOvertimeInfo){
                if (ImGui::TreeNode(p.first.c_str())){
                    ImGui::PushItemWidth(60);
                    TimedBlockOvertimeInfo& i = p.second;
                    ImGui::LabelText("Average", "%.0fms", i.average);
                    ImGui::LabelText("Min", "%.0fms", i.min);
                    ImGui::LabelText("Max", "%.0fms", i.max);
                    ImGui::LabelText("Records", "%dms", i.records);
                    ImGui::PopItemWidth();
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        //ImGui::TreePop();
        //ImGui::InputText("Test", buf, 255);
        //ImGui::InputTextMultiline("t2", (char*)o.str().c_str(), 10000, ImVec2(600, 600), ImGuiInputTextFlags_ReadOnly);
    ImGui::End();

    ImGui::Begin("Console");
        ImGui::InputTextMultiline("##console", (char*)ossConsole.str().c_str(), ossConsole.str().size(), ImVec2(500, 600), ImGuiInputTextFlags_ReadOnly);
        ImGui::PushItemWidth(500);
        if (ImGui::InputText("##command_line", commandLine, 1000, ImGuiInputTextFlags_EnterReturnsTrue)){
            ossConsole << commandLine << std::endl;
            commandLine[0] = '\0';
        }
        ImGui::PopItemWidth();
    ImGui::End();
}

void DebugServices::pushMessage(std::string message, std::string type, std::string fileName, int lineNumber, std::string functionName){
    MessageRecord record;
    record.fileName = fileName;
    record.lineNumber = lineNumber;
    record.functionName = functionName;
    record.fullAddress = getFormatedAdress(fileName, lineNumber, functionName);
    record.type = type;
    record.timeStamp = diffclock(clock(), clockDebugStart);

    record.message = message;

    messagesLists[type + "s"].push_back(record);
    if (messagesLists[type + "s"].size() > maxStoredMessages) messagesLists[type + "s"].pop_front();

    ossConsole << record.fullAddress << " -- " << record.type << ": " << record.message << std::endl;
}

void DebugServices::enableFilePersistentTimedBlockOutput(std::string fileName, std::ostream* outStream){
    std::map<std::string, std::ostream*>::iterator it = mapFilePersistentTimedBlockOutput.find(fileName);
    if (it == mapFilePersistentTimedBlockOutput.end()){
        mapFilePersistentTimedBlockOutput.insert(std::make_pair(fileName, (outStream ? outStream : &outputStream)));
    }else{
        it->second = outStream;
    }
}

void DebugServices::enableFunctionPersistentTimedBlockOutput(std::string fileName, std::string functionName, std::ostream* outStream){
    std::map<std::string, std::map<std::string, std::ostream*> >::iterator it =
                                                                  mapFunctionPersistentTimedBlockOutput.find(fileName);

    mapFunctionPersistentTimedBlockOutput[fileName][functionName] = (outStream ? outStream : &outputStream);
}

void DebugServices::enableFunctionPersistentWatchedVaribleOutput(std::string fileName, std::string functionName, std::ostream* outStream){
    std::map<std::string, std::map<std::string, std::ostream*> >::iterator it =
                                                                  mapFunctionPersistentWatchedVariableOutput.find(fileName);

    mapFunctionPersistentWatchedVariableOutput[fileName][functionName] = (outStream ? outStream : &outputStream);
}

double DebugServices::diffclock(std::clock_t c1, std::clock_t c2){
    return (c1 - c2) / (double) CLOCKS_PER_SEC;
}

void DebugServices::_addTimedBlockRecord(_TimedBlock& block){
    TimedBlockRecord record;
    record.fileName = block.fileName;
    record.lineNumber = block.lineNumber;
    record.functionName = block.functionName;
    record.fullID = getFormatedAdress(block.fileName, block.lineNumber, block.functionName);
    //record.recordEntry = timedBlockRecords[record.fullID].size()+1;
    record.timeSpent = 0;
    record.startTimeStamp = diffclock(block.start, clockDebugStart)*1000;
    record.stopTimeStamp = diffclock(clock(), clockDebugStart)*1000;
    record.timeSpent = record.stopTimeStamp - record.startTimeStamp;

/*
    std::map<std::string, std::ostream*>::iterator it = mapFilePersistentTimedBlockOutput.find(fileName);
    if (it != mapFilePersistentTimedBlockOutput.end()){
        std::ostream& outputStream = *it->second;
        outputStream //<< std::setw(6) << record.recordEntry << " "
                     << record.fileName << " "
                     << "(" << std::setw(4) << record.lineNumber << ") "
                     << record.functionName
                     << ": Time spent = " << std::setprecision(6) << record.timeSpent
                     << std::endl;
    }
*/
    TimedBlockOvertimeInfo& overtimeInfo = timedBlocksOvertimeInfo[record.fullID];
    overtimeInfo.fileName = block.fileName;
    overtimeInfo.lineNumber = block.lineNumber;
    overtimeInfo.functionName = block.functionName;
    overtimeInfo.max = std::max(record.timeSpent, overtimeInfo.max);
    overtimeInfo.min = std::min(record.timeSpent, overtimeInfo.min);
    overtimeInfo.sum += record.timeSpent;
    overtimeInfo.records++;
    overtimeInfo.records = std::min(overtimeInfo.records, (int)maxStoredTimedBlockEntries);
    overtimeInfo.average = overtimeInfo.sum/(double)overtimeInfo.records;

    timedBlockRecords[record.fullID].push_back(record);
    if (timedBlockRecords[record.fullID].size() >= maxStoredTimedBlockEntries){
        timedBlocksOvertimeInfo[record.fullID].sum -= timedBlockRecords[record.fullID].front().timeSpent;
        timedBlockRecords[record.fullID].pop_front();

    }
}

_TimedBlock::_TimedBlock(std::string fileName, int lineNumber, std::string functionName){
    this->fileName = fileName;
    this->lineNumber = lineNumber;
    this->functionName = functionName;
    this->start = std::clock();
}

_TimedBlock::~_TimedBlock(){
    DebugServices::get()._addTimedBlockRecord(*this);
}

std::string DebugServices::getFileName(std::string path){
    std::string fileName;
    unsigned int i;
    for (i = path.size()-1; i >= 0; i--){
        if (path[i] == '\\' || path[i] == '/'){
            i++;
            break;
        }
    }
    for ( ; i < path.size(); i++){
        fileName += path[i];
    }
    return fileName;
}

std::string DebugServices::getFormatedAdress(std::string fileName, int lineNumber, std::string functionName){
    std::stringstream ss;
    ss << fileName << "(" << std::setw(4) << lineNumber << "): " << functionName;
    return ss.str();
}

void DebugServices::pause(){
    isPaused = true;
}

void DebugServices::unpause(){
    isPaused = false;
}

void DebugServices::outputToConsole(std::string message){
    ossConsole << message;
}

