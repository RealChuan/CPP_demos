add_executable(mysql mysql.cpp)
target_link_libraries(mysql mysqlclient)

add_executable(sqlite sqlite.cpp)
target_link_libraries(sqlite sqlite3)
