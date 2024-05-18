#ifndef TOOL_TYPES_H_
#define TOOL_TYPES_H_


#include <string>

enum ToolType {
    Brush,
    Select,
    Eraser,
    TOOL_COUNT = 3
};

std::string ToolTypeToString(ToolType toolType);

#endif