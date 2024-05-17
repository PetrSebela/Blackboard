#ifndef TOOL_TYPES_H_
#define TOOL_TYPES_H_


#include <string>

enum ToolType {
    Brush,
    Select,
    TOOL_COUNT = 2
};

std::string ToolTypeToString(ToolType toolType);

#endif