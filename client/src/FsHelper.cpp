#include "../include/FsHelper.hpp"
#include <mutex>

std::once_flag FsHelper::m_init_flag;
std::unique_ptr<FsHelper> FsHelper::m_instance;

FsHelper& FsHelper::get_instance(){
    std::call_once(m_init_flag, [](){
        m_instance = std::make_unique<FsHelper>();
        const char* home = std::getenv("HOME");
        m_instance->m_working_path = home ? home : "/";
    });
    return *m_instance;
}

std::filesystem::path FsHelper::cwd() const {
    return m_working_path.string();
}

void FsHelper::set_cwd(const std::filesystem::path& cwd){
    m_working_path = cwd;
}