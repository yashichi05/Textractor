macro(msvc_registry_search)
# 沒有Qt5_DIR、MSVC 變數
	if(NOT DEFINED Qt5_DIR AND MSVC)
		# look for user-registry pointing to qtcreator 取得 qtcreater 的bin資料夾(C:\Qt\\Tools\QtCreator\bin\qtcreator.exe -client "%1") ，PATH 同 DIRECTORY 僅取得資料夾
		# https://cmake.org/cmake/help/v3.0/command/get_filename_component.html
		get_filename_component(QT_BIN [HKEY_CURRENT_USER\\Software\\Classes\\Applications\\QtProject.QtCreator.pro\\shell\\Open\\Command] PATH)

		# get root path so we can search for 5.3, 5.4, 5.5, etc
		# string(REPLACE 找的文字 取代成的文字 輸出的變數名 輸入字串)  -> C:/Qt/;/QtCreator/bin
		string(REPLACE "/Tools" ";" QT_BIN "${QT_BIN}")
		# 取得陣列的N個資料，list(GET 輸入陣列資料 第幾個 輸出變數) ->  C:/Qt/
		list(GET QT_BIN 0 QT_BIN)
		# 取得 5.1 版資料夾陣列
		file(GLOB QT_VERSIONS "${QT_BIN}/5.1*")
		list(SORT QT_VERSIONS)

		# assume the latest version will be last alphabetically
		list(REVERSE QT_VERSIONS)
		# 取得 5.1最新版
		list(GET QT_VERSIONS 0 QT_VERSION)

		# fix any double slashes which seem to be common
		string(REPLACE "//" "/"  QT_VERSION "${QT_VERSION}")
		# https://cmake.org/cmake/help/latest/variable/MSVC_VERSION.html
		# 設定msvc 資料夾版本名
		if(MSVC_VERSION GREATER_EQUAL 1920)
			set(QT_MSVC 2019)
		elseif(MSVC_VERSION GREATER_EQUAL 1910)
			set(QT_MSVC 2017)
		elseif(MSVC_VERSION GREATER_EQUAL 1900)
			set(QT_MSVC 2015)
		else()
			message(WARNING "Unsupported MSVC toolchain version")
		endif()
		# 設定64 或 32
		if(QT_MSVC)
			if(CMAKE_CL_64)
				SET(QT_SUFFIX "_64")
			else()
				set(QT_SUFFIX "")
			endif()

			# MSVC 2015+ is only backwards compatible
			# QT5 find package時，會去找Qt5_DIR變數，設定 Qt5_DIR 變數
			if(EXISTS "${QT_VERSION}/msvc${QT_MSVC}${QT_SUFFIX}")
				set(Qt5_DIR "${QT_VERSION}/msvc${QT_MSVC}${QT_SUFFIX}/lib/cmake/Qt5")
			elseif(QT_MSVC GREATER_EQUAL 2019 AND EXISTS "${QT_VERSION}/msvc2017${QT_SUFFIX}")
				set(Qt5_DIR "${QT_VERSION}/msvc2017${QT_SUFFIX}/lib/cmake/Qt5")
			elseif(QT_MSVC GREATER_EQUAL 2017 AND EXISTS "${QT_VERSION}/msvc2015${QT_SUFFIX}")
				set(Qt5_DIR "${QT_VERSION}/msvc2015${QT_SUFFIX}/lib/cmake/Qt5")
			else()
				message(WARNING "Required QT5 toolchain is not installed")
			endif()
		endif()
	endif()
endmacro()

macro(find_qt5)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	# AUTOMOC > 編譯Q_OBJECT用
	#set(CMAKE_AUTOMOC ON)
	# 自動處理qt
	set(CMAKE_AUTOUIC ON)
	# 新增編譯變數 -D
	add_definitions(-DQT_DEPRECATED_WARNINGS -DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
	# argn 巨集傳入的arguments(Core Widgets WebSockets)
	# 設qt5 的COMPONENTS
	find_package(Qt5 COMPONENTS ${ARGN})

	# https://cmake.org/cmake/help/v3.0/prop_gbl/PACKAGES_FOUND.html 找到package
	if(Qt5_FOUND)
	# 是否為WIN32， Qt5::qmake -> qt5 核心載入後 增加的target
	# TARGET 由這些建立 add_executable(), add_library(), or add_custom_target() 是否存在。
		if(WIN32 AND TARGET Qt5::qmake AND NOT TARGET Qt5::windeployqt)
		# 取得資料夾 設為_qt5_qmake_location 變數qt5_install_prefix 變數
			get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)
		# 輸出結果 命名為qt5_install_prefix
			execute_process(
				COMMAND "${_qt5_qmake_location}" -query QT_INSTALL_PREFIX
				RESULT_VARIABLE return_code
				OUTPUT_VARIABLE qt5_install_prefix
				OUTPUT_STRIP_TRAILING_WHITESPACE
			)

			set(imported_location "${qt5_install_prefix}/bin/windeployqt.exe")

			if(EXISTS ${imported_location})
			# 匯入執行檔(qt 發佈器，用來複製qt所需dll)，IMPORTED 引用專案外的target
				add_executable(Qt5::windeployqt IMPORTED)
			# 設定 PROPERTIES IMPORTED_LOCATION 匯入路徑
				set_target_properties(Qt5::windeployqt PROPERTIES
					IMPORTED_LOCATION ${imported_location}
				)
			endif()
		endif()
	else()
		message(FATAL_ERROR "Cannot find QT5!")
	endif()
endmacro(find_qt5)
