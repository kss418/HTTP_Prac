#pragma once
#include <memory>

class FsHelper{
public:
    static FsHelper& get_instance();

    FsHelper(const FsHelper&) = delete;
    FsHelper& operator=(const FsHelper&) = delete;
    FsHelper(FsHelper&&) = delete;
    FsHelper& operator=(FsHelper&&) = delete;
private:
    FsHelper() = default;
    
    static std::once_flag m_init_flag;
    static std::unique_ptr<FsHelper> m_instance;
};