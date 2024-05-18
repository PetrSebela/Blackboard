#include "tool_types.hpp"

std::string ToolTypeToString(ToolType toolType)
{
    switch (toolType)
    {
    case ToolType::Brush: return "Brush";
    case ToolType::Select: return "Select";
    case ToolType::Eraser: return "Eraser";
    default: return "Unsupported tool";
    }
}