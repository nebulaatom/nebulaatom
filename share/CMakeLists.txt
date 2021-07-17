# .desktop file
configure_file(
	project.desktop.in 
	${PROJECT_BINARY_DIR}/${PROJECT_NAME}.desktop
)

# .glade file
configure_file(
	project.glade 
	${PROJECT_BINARY_DIR}/share/${PROJECT_NAME}/${PROJECT_NAME}.glade
)

# .pc file
configure_file(
	project.pc.in
	${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc
	@ONLY
)
