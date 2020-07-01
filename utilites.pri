### function messenger("message") #############################################
###### This wraps all message calls so they can be silenced with one comment
defineTest(messenger) {
    isEmpty(MY_SILENT_MESSAGE) {
        message($$1)
    }
}
### function checkAllFilesExist(file_list) ####################################
defineTest(checkAllFilesExist) {
    files = $$1
    for(FILE, files) {
        !exists($$FILE) {
            error("File does Not Exist -> $$FILE")
        }
    }
}
### function ifNotExistFolderCreate
# Create folders if they do not exist
# Windows Users:
# If you can not run mkdir in a terminal see the below link
# https://www.windowscentral.com/install-windows-subsystem-linux-windows-10
defineTest(ifNotExistFolderCreate) {
    theFolder = $$1
    exists($$theFolder) {
            messenger("Folder Exists -> $$theFolder")
        } else {
                QMAKE_POST_LINK += $$quote(mkdir $${theFolder} $$escape_expand(\n\t))
                export(QMAKE_POST_LINK)
                messenger("Created Folder -> $$theFolder")
    }
}
### function Show Qt Info #####################################################
defineTest(showQtInfo) {
    messenger("***-> ************* Show Qt Info Begin ************* <-***")
    messenger(Qt version: $$[QT_VERSION])
    messenger(Qt is installed in $$[QT_INSTALL_PREFIX])
    messenger(Qt resources can be found in the following locations:)
    messenger(Documentation: $$[QT_INSTALL_DOCS])
    messenger(Header files: $$[QT_INSTALL_HEADERS])
    messenger(Libraries: $$[QT_INSTALL_LIBS])
    messenger(Binary files (executables): $$[QT_INSTALL_BINS])
    messenger(Plugins: $$[QT_INSTALL_PLUGINS])
    messenger(Data files: $$[QT_INSTALL_DATA])
    messenger(Translation files: $$[QT_INSTALL_TRANSLATIONS])
    messenger(Settings: $$[QT_INSTALL_SETTINGS])
    messenger(Examples: $$[QT_INSTALL_EXAMPLES])
    messenger(Demonstrations: $$[QT_INSTALL_DEMOS])
    messenger("***<- ************* Show Qt Info End ************* ->***")
}
#showQtInfo("")
### function copyToDestDir ####################################################
defineTest(copyToDestDir) {
    files = $$1

    for(FILE, files) {
        DDIR = $$DESTDIR
		FILE = $$absolute_path($$FILE)

        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}
################################ End of File ##################################
