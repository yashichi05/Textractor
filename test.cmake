get_filename_component(QT_BIN [HKEY_CURRENT_USER\\Software\\Classes\\Applications\\QtProject.QtCreator.pro\\shell\\Open\\Command] PATH)
execute_process(
	COMMAND "C:\Qt\5.15.2\mingw81_32\bin\qmake.exe" -query QT_INSTALL_PREFIX
	RESULT_VARIABLE return_code
	OUTPUT_VARIABLE qt5_install_prefix
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "${qt5_install_prefix}")
string(REPLACE "/Tools" ";" QT_BIN "${QT_BIN}")
list(GET QT_BIN 0 QT_BIN)
file(GLOB ASSETS *)
message(STATUS "${MSVC_VERSION}")
