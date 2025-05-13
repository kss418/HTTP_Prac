#include "../include/FsHelper.hpp"
#include <mutex>

std::once_flag FsHelper::m_init_flag;
std::unique_ptr<FsHelper> FsHelper::m_instance;

FsHelper::FsHelper(){}

FsHelper& FsHelper::get_instance(){
    std::call_once(m_init_flag, [](){
        m_instance = std::make_unique<FsHelper>();
    });
    return *m_instance;
}