cmake_minimum_required(VERSION 3.11)
project(yap)
include(FetchContent)
 
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9
)
 
FetchContent_MakeAvailable(ftxui)
 
add_executable(yap main.cpp yandex_music_app.cpp app_factory.cpp http_client.cpp)
target_link_libraries(yap
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE crypto
  PRIVATE ssl
)

