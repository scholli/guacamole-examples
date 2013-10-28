# copy runtime libraries as a post-build process
SET ( _COPY_COMMAND "copy" )
SET ( _PUSH_DIRECTORY_COMMAND "pushd" )
SET ( _POP_DIRECTORY_COMMAND "popd" )

# gua_copy_runtime_dependencies [target_name] [path_to_runtime_libraries] [executable_path]
MACRO(gua_copy_runtime_dependencies)
	SET(_TARGET_NAME ${ARGV0})
	SET(_RUNTIME_LIBRARY_PATH ${ARGV1})
    SET(_TARGET_PATH ${ARGV2})
	
	IF (WIN32)
		SET(_POST_PROCESS_COMMAND
			 ${_PUSH_DIRECTORY_COMMAND} ${_RUNTIME_LIBRARY_PATH} \n
			 ${_COPY_COMMAND} *.dll ${_TARGET_PATH} \n
			 ${_POP_DIRECTORY_COMMAND})
		ADD_CUSTOM_COMMAND ( TARGET ${_TARGET_NAME} POST_BUILD COMMAND ${_POST_PROCESS_COMMAND})
	ENDIF(WIN32)
ENDMACRO(gua_copy_runtime_dependencies)