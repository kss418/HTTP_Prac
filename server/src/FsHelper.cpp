#include "../include/FsHelper.hpp"

std::once_flag FsHelper::m_init_flag;
std::unique_ptr<FsHelper> FsHelper::m_instance;

FsHelper& FsHelper::get_instance(){
    std::call_once(m_init_flag, [](){
        m_instance = std::make_unique<FsHelper>();
    });
    return *m_instance;
}

FsExecuter::FsExecuter(const std::string& id)
  :  m_working_path(std::filesystem::current_path().root_directory() / "home" / "kss418" / id){  
}

std::filesystem::path FsExecuter::cwd() const{
    return m_working_path.string();
}

bool FsExecuter::exists(const std::filesystem::path& cwd) const{
    return std::filesystem::exists(cwd);
}

void FsExecuter::set_cwd(const std::filesystem::path& cwd){
    m_working_path = cwd;
}

bool FsExecuter::mkdir(const std::filesystem::path& cwd){
    const auto path = cwd.is_absolute() ? cwd : (m_working_path / cwd);
    return !std::filesystem::create_directories(path);
}

void FsExecuter::rmdir(const std::filesystem::path& cwd){
    const auto path = cwd.is_absolute() ? cwd : (m_working_path / cwd);
    if(!std::filesystem::exists(path)){
        // std::cout << "대상 디렉토리가 존재하지 않습니다." << std::endl;
    }
    else if(!std::filesystem::is_directory(path)){
        // std::cout << "대상이 디렉토리가 아닙니다." << std::endl;
    }
    else if(!std::filesystem::remove_all(path)){
        // std::cout << "빈 디렉토리가 아닙니다." << std::endl;
    }
}

void FsExecuter::ls() const{

}
