#include "tool_types.hpp"

std::string ToolTypeToString(ToolType toolType)
{
    switch (toolType)
    {
    case ToolType::Brush: return "Brush";
    case ToolType::Select: return "Select";
    default: return "Unsupported tool";
    }
}