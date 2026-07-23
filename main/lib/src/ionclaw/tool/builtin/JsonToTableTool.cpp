  #include "ionclaw/tool/builtin/JsonToTableTool.hpp"

  #include <algorithm>
  #include <iomanip>
  #include <map>
  #include <sstream>
  #include <vector>

  namespace ionclaw::tool::builtin {

  ToolResult JsonToTableTool::execute(const nlohmann::json
  &params, const ToolContext &/*context*/)
  {
      // 1. 取出 data 参数
      if (!params.contains("data") || !params["data"].is_array())
      {
          return "Error: 'data' must be a JSON array";
      }

      const auto &data = params["data"];

      if (data.empty())
      {
          return "(empty array)";
      }

      // 2. 确定列：优先用 columns 参数，否则取第一行对象的 key
      std::vector<std::string> columns;

      if (params.contains("columns") &&
  params["columns"].is_array())
      {
          for (const auto &col : params["columns"])
          {
              columns.push_back(col.get<std::string>());
          }
      }
      else
      {
          // 从第一行提取 key，保持插入顺序
          // nlohmann::json 的 items() 按插入顺序迭代
          for (const auto &[key, _] : data[0].items())
          {
              columns.push_back(key);
          }
      }

      // 3. 计算每列最大宽度
      std::map<std::string, size_t> widths;
      for (const auto &col : columns)
      {
          widths[col] = col.size(); // 列名本身
      }
      for (const auto &row : data)
      {
          for (const auto &col : columns)
          {
              std::string val = row.value(col, "");
              widths[col] = std::max(widths[col], val.size());
          }
      }

      // 4. 拼 Markdown 表格
      std::ostringstream oss;

      // --- 表头 ---
      for (size_t i = 0; i < columns.size(); i++)
      {
          oss << "| " << std::left <<
  std::setw(static_cast<int>(widths[columns[i]])) << columns[i] <<
  " ";
      }
      oss << "|\n";

      // --- 分隔行 ---
      for (size_t i = 0; i < columns.size(); i++)
      {
          oss << "|" << std::string(widths[columns[i]] + 2, '-');
      }
      oss << "|\n";

      // --- 数据行 ---
      for (const auto &row : data)
      {
          for (size_t i = 0; i < columns.size(); i++)
          {
              std::string val = row.value(columns[i], "");
              oss << "| " << std::left <<
  std::setw(static_cast<int>(widths[columns[i]])) << val << " ";
          }
          oss << "|\n";
      }

      return oss.str();
  }

  ToolSchema JsonToTableTool::schema() const
  {
      return {
          "json_to_table",
          "Convert a JSON array into a formatted Markdown table. "
          "Useful for presenting structured data to the user.",
          {
              {"type", "object"},
              {"properties",
               {
                   {"data",
                    {
                        {"type", "array"},
                        {"description", "JSON array of objects, each object is one row"}
                    }
                   },
                   {"columns",
                    {
                        {"type", "array", "items", {{"type",
  "string"}}},
                        {"description", "Optional: specify column names and order (default: all keys from first row)"}
                    }
                   }
               }
              },
              {"required", nlohmann::json::array({"data"})}
          }
      };
  }

  }