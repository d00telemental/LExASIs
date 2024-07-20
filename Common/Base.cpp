#include "Common/Base.hpp"

void Common::InitializeLoggerDefault()
{
    spdlog::default_logger()->set_pattern("%^[%H:%M:%S.%e] [%l] %v%$");
    spdlog::default_logger()->set_level(spdlog::level::trace);
}

void Details::OnFatalError(char const* const Func, char const* const File, int const Line, char const* const Expr)
{
    char const* const ExprSafe = (Expr != nullptr && strlen(Expr) > 0) ? Expr : "???";
    spdlog::source_loc const Location{ File, Line, Func };
    spdlog::log(Location, spdlog::level::critical, "Fatal assertion: {}", ExprSafe);
    spdlog::log(Location, spdlog::level::critical, "  at: {}:{}", File, Line);
    spdlog::log(Location, spdlog::level::critical, "  func: {}", Func);
    std::abort();
}
