
# --- FIND MYSQL ---

# Find include dirs
find_path(MARIADB_INCLUDE_DIRS
	NAMES mysql.h
	PATH_SUFFIXES mariadb
	PATHS /usr/include/mysql
		/usr/local/include/mysql
		/usr/include/mariadb
		/usr/local/include/mariadb
		/opt/mysql/mysql/include
		/opt/mysql/mysql/include/mysql
		/opt/mysql/include
		/opt/local/include/mysql5
		/usr/local/mysql/include
		/usr/local/mysql/include/mysql
		/usr/local/mariadb/include
		/usr/local/mariadb/include/mariadb
		/opt/rh/rh-mariadb105/root/usr/include
		/opt/rh/rh-mariadb105/root/usr/include/mysql
		$ENV{ProgramFiles}/MySQL/*/include
		$ENV{SystemDrive}/MySQL/*/include
)

find_path(MYSQL_INCLUDE_DIRS
	NAMES mysql.h
	PATH_SUFFIXES mysql
	PATHS /usr/include/mysql
		/usr/local/include/mysql
		/usr/include/mariadb
		/usr/local/include/mariadb
		/opt/mysql/mysql/include
		/opt/mysql/mysql/include/mysql
		/opt/mysql/include
		/opt/local/include/mysql5
		/usr/local/mysql/include
		/usr/local/mysql/include/mysql
		/usr/local/mariadb/include
		/usr/local/mariadb/include/mariadb
		/opt/rh/rh-mariadb105/root/usr/include
		/opt/rh/rh-mariadb105/root/usr/include/mysql
		$ENV{ProgramFiles}/MySQL/*/include
		$ENV{SystemDrive}/MySQL/*/include
)

# Set include dirs
if(EXISTS "${MARIADB_INCLUDE_DIRS}/mysql.h")
	set(MYSQL_INCLUDE_DIRS ${MARIADB_INCLUDE_DIRS})
elseif(EXISTS "${MYSQL_INCLUDE_DIRS}/mysql/mysql.h")
	set(MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIRS}/mysql)
endif()

# Find library dirs
find_library(MYSQL_LIBRARIES
	NAMES mysqlclient_r mariadbclient mariadb
	PATHS /usr/lib/mysql
			/usr/lib/mariadb
			/usr/local/lib/mysql
			/usr/local/lib/mariadb
			/usr/local/mysql/lib
			/usr/local/mysql/lib/mysql
			/opt/local/mysql5/lib
			/opt/local/lib/mysql5/mysql
			/opt/mysql/mysql/lib/mysql
			/opt/mysql/lib/mysql
			/opt/rh/rh-mariadb105/root/usr/lib64
)

# Verify if exists directories
if(MYSQL_INCLUDE_DIRS AND MYSQL_LIBRARIES)
	message(STATUS "MySQL Include dir: ${MYSQL_INCLUDE_DIRS}")
	message(STATUS "MySQL client libraries: ${MYSQL_LIBRARIES}")
elseif(MySQL_FIND_REQUIRED)
	message(FATAL_ERROR "Cannot find MySQL. Include dir: ${MYSQL_INCLUDE_DIRS}  library dir: ${MYSQL_LIBRARIES_DIR}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MySQL
	DEFAULT_MSG
	MYSQL_LIBRARIES
	MYSQL_INCLUDE_DIRS
)

# Copy the results to the output variables.
if(MySQL_FOUND)
	add_library(MySQL_lib INTERFACE IMPORTED)
	set_target_properties(MySQL_lib
		PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${MYSQL_INCLUDE_DIRS}"
		INTERFACE_LINK_LIBRARIES "${MYSQL_LIBRARIES}"
	)
else()
	set(MYSQL_LIBRARIES)
	set(MYSQL_INCLUDE_DIRS)
endif()

mark_as_advanced(MYSQL_INCLUDE_DIRS MYSQL_LIBRARIES)